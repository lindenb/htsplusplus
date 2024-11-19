#include <cstdlib>
#include "GetOpt.hh"
#include "SamFileReaderFactory.hh"
#include "SamRecordIterator.hh"
#include "Rimbert20241119.hh"
#include "DiscreteMedian.hh"

using namespace htspp;
using namespace std;


Rimbert20241119::Rimbert20241119():Program("rimbert20241119","find sv in region") {
}

Rimbert20241119::~Rimbert20241119() {
}


int Rimbert20241119::main(int argc, char** argv) {
GetOpt getopt;
char* fasta=NULL;
char* bam_file=NULL;
char* bai_file = NULL;
char* opt_reg = NULL;
getopt.cStringOpt('T',&fasta,"indexed fasta reference").longOpt("reference").required();
getopt.cStringOpt('b',&bam_file,"Bam").longOpt("bam").required();
getopt.cStringOpt('i',&bai_file,"BAI").longOpt("bai");
getopt.cStringOpt('r',&opt_reg,"interval").longOpt("interval").required();
if(getopt.parse(argc,argv)!=0) {
    getopt.usage(cerr);
	return EXIT_FAILURE;
	}

SamFileReaderFactory sfr;
sfr.reference(fasta);
unique_ptr<SamFileReader> sr = sfr.open(bam_file,bai_file);
auto reg = sr->dictionary()->parseInterval(opt_reg);
unique_ptr<SamRecordIterator> iter= sr->query_by_loc(reg.get());
SamRecord rec;
vector<AlignmentBlock> alignmentBlocks;
vector<int32_t> coverage(reg->length_on_reference(), 0);
while(iter->read2(&rec)) {
	rec.alignment_blocks(alignmentBlocks);
	for(size_t i=0;i< alignmentBlocks.size();++i) {
		AlignmentBlock& ab = alignmentBlocks[i];
		for(int x=0; x < ab.length;++x) {
			int refpos = ab.referenceStart +x;
			if(!reg->contains_position(refpos)) continue;
			coverage[refpos-reg->start()]++;
			}
		}
	}
DiscreteMedian<int> dm;
for(auto i=coverage.begin();i!=coverage.end();i++) {
    dm.accept(*i);
    }
Optional<double> median_obj = dm.median();

double median = median_obj.get();
#define is_dup(DP) (DP>=median*1.4)
#define is_del(DP) (DP<=median*0.6)
size_t i=0;
while(i< coverage.size()) {
    int dp = coverage[i];
    if(is_dup(dp)) {
        size_t beg = i;
        while(i< coverage.size() && is_dup(coverage[i])) {
            i++;
            }
        fprintf(stdout,"%d:%d-%d\tDUP\n",beg+reg->start(),i+reg->start());
        }
    else if(is_del(dp)) {
        size_t beg = i;
        double sum=0.0;
        while(i< coverage.size() && is_del(coverage[i])) {
            sum+= coverage[i];
            i++;
            }
        fprintf(stdout,"%d:%d-%d\tDEL\t%f\t%f\t%f\n",beg+reg->start(),i+ reg->start(), sum/(i-beg),median,median*0.6);
        }
    else  {
        i++;
        }
    }
return 0;
}

