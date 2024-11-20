/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#include <cmath>
#include <cstdlib>
#include "GetOpt.hh"
#include "SamFileReaderFactory.hh"
#include "SamRecordIterator.hh"
#include "Rimbert20241119.hh"
#include "DiscreteMedian.hh"
#include "CoordMath.hh"
#include "ANSI.hh"
using namespace htspp;
using namespace std;


struct BinInfo {

};

struct CNV {
int start;
int end;
int svtype;
};


static double median(int* array0,int winsize) {
	int array[winsize];
	memcpy(array,array0,sizeof(int)*(winsize));
	std::sort(array, array+winsize, [](const double &a, const double &b) {return a < b;});
	if(winsize%2==1) {
		return array[winsize/2];
		}
	else
		{
		return (array[winsize/2-1]+array[winsize/2])/2.0;
		}
	return 0;
	}

static void runningMedian(vector<int>& coverage,int winsize) {
    vector<int> copy(coverage.begin(),coverage.end());
    int array[winsize];
    int half_win=winsize/2;
    int idx=0;
    size_t i;
    if(winsize<2) return;
    for(i=0;i < coverage.size();i++) {
        array[idx]=coverage[i];
        idx++;
        
        if(idx==winsize) {
        		if(i+half_win< coverage.size()) {
        			copy[i+half_win] =  (int)median(array,idx);
        			}
		        memmove(&array[0],&array[1],sizeof(int)*(winsize-1));
            idx--;
            }
				}
    coverage.swap(copy);
    }

static double average(vector<double>& coverage,int a,int b) {
	double sum=0;
	int count=0;
	while(a<b && a < (int)coverage.size()) {
		sum+=coverage[a++];
		++count;
		}
	return (sum/count);
  }

Rimbert20241119::Rimbert20241119():Program("rimbert20241119","find sv in region by looking at the median depth") {
}

Rimbert20241119::~Rimbert20241119() {
}


int Rimbert20241119::main(int argc, char** argv) {
GetOpt getopt;
bool ansi_blocks=false;
int opt_n_bins=100;
double treshold=0.4;
int run_median_win_size=7;
int winthin_distance=20;
int min_cnv_size=100;
char* fasta=NULL;
char* bam_file=NULL;
char* bai_file = NULL;
char* opt_reg = NULL;
int mapq=1;
getopt.boolOpt('B',&ansi_blocks).description("plot coverage as ascii").longOpt("blocks");
getopt.stringOpt('T',&fasta).description("indexed fasta reference").longOpt("reference");
getopt.stringOpt('r',&opt_reg).description("interval").longOpt("interval").required();
getopt.intOpt('d',&winthin_distance).description("merge interval withing distance 'x'").longOpt("merge");
getopt.intOpt('m',&run_median_win_size).description("running median window size").longOpt("running-median-length");
getopt.intOpt('Q',&mapq).description("min mapq").longOpt("mapq");
getopt.intOpt('s',&min_cnv_size).description("min CNV size").longOpt("min-size");
getopt.doubleOpt('f',&treshold).description("CNV treshold  DUP > 1.0+x  DEL =< 1.0-x ").longOpt("treshold");
getopt.intOpt('w',&opt_n_bins).description(" number of bins").longOpt("n-bins");

if(getopt.parse(argc,argv)!=0) {
  this->usage(cerr,&getopt);
	return EXIT_FAILURE;
	}
if(getopt.files.size()==1 ) {
	bam_file = getopt.files[0];
	bai_file = NULL;
	}
else if(getopt.files.size()==2 ) {
	bam_file = getopt.files[0];
	bai_file = getopt.files[1];
	}
else
	{
  cerr <<  ANSI_RED("illegal number of arguments") << endl;
	return EXIT_FAILURE;
	}


SamFileReaderFactory sfr;
sfr.reference(fasta);
unique_ptr<SamFileReader> sr = sfr.open(bam_file,bai_file,true);
auto reg = sr->dictionary()->parseInterval(opt_reg);
if(reg->length_on_reference() <opt_n_bins) {
	opt_n_bins = reg->length_on_reference();
	WARNING("interval is too short. Adjusting bin to " << opt_n_bins);
	}
unique_ptr<SamRecordIterator> iter= sr->query_by_loc(reg.get());
SamRecord rec;
vector<AlignmentBlock> alignmentBlocks;
vector<int32_t> coverage(opt_n_bins,0);
double base_per_bin = (reg->length_on_reference()/opt_n_bins);

#define TO_BIN(X)  ((int)(((X - reg->start() )/(double)reg->length_on_reference())  * opt_n_bins))

int n_reads=0;
while(iter->read2(&rec)) {
	if(rec.is_ignorable(mapq)) continue;
	rec.alignment_blocks(alignmentBlocks);
	n_reads++;
	hts_pos_t prev_pos=-1;
	for(size_t i=0;i< alignmentBlocks.size();++i) {
		AlignmentBlock& ab = alignmentBlocks[i];
		for(int x=0; x < ab.length;++x) {
			hts_pos_t refpos = ab.referenceStart +x;
			if(refpos <= prev_pos) continue;
			prev_pos = refpos;
			if(!reg->contains_position(refpos)) {
				continue;
				}
			int bin_idx= TO_BIN(refpos);

			if(bin_idx<0 || bin_idx>=(int)coverage.size()) {
				continue;
				}
			coverage[bin_idx]++;
			}
		}
	}

runningMedian(coverage,run_median_win_size);	
	
DiscreteMedian<int> dm;
for(auto i=coverage.begin();i!=coverage.end();i++) {
		double x  = ceil((*i)/(double)base_per_bin);
		*i = (int)x;
    dm.accept(x);
    }

Optional<double> median_obj = dm.median();
if(!median_obj.isPresent()) {
	cerr <<  ANSI_RED("no median can be exracted") << std::endl;
	return EXIT_FAILURE;
	}


double median = median_obj.get();

vector<double> normalized(coverage.size(),0.0);
for(size_t i=0;i< coverage.size();i++) {
	normalized[i] = (median==0?coverage[i]:coverage[i]/median);
	/*
		int n = (int)(normalized[i]*20.0);
	for(int k=0;k< n;k++) cout << "*";
	cout << " " << normalized[i]  << ":" << coverage[i] << endl;
	*/
}


#define TO_COORD(X) (reg->start()+(X*base_per_bin))
#define is_dup(DP) (DP>(1.0+treshold))
#define is_del(DP) (DP<=(1.0-treshold))
vector<CNV> cnvs_list;
/** find CNVs */
size_t i=0;
while(i< normalized.size()) {
    double dp = normalized[i];
    if(is_dup(dp)) {
        CNV cnv;
        cnv.start = TO_COORD(i);
        while(i< normalized.size() && is_dup(normalized[i])) {
            i++;
            }
        cnv.end = TO_COORD(i);
        cnv.svtype=1;
        if(cnv.end - cnv.start >= min_cnv_size) cnvs_list.push_back(cnv);
        }
    else if(is_del(dp)) {
        CNV cnv;
        cnv.start = TO_COORD(i);
        while(i< normalized.size() && is_del(normalized[i])) {
            i++;
            }
        cnv.end = TO_COORD(i);
        cnv.svtype=-1;
        if(cnv.end - cnv.start >= min_cnv_size) cnvs_list.push_back(cnv);
        }
    else  {
        i++;
        }
    }
/* merge adjacent SNVs */
i=0;
while(i< coverage.size()) {
	CNV& sv0 = cnvs_list[i];
	CNV* next = i+1 < cnvs_list.size()?&cnvs_list[i+1]:NULL;
	if(next!=NULL && sv0.svtype== next->svtype && CoordMath::overlaps(sv0.start-winthin_distance,sv0.end+winthin_distance,next->start,next->end) ) {
	  sv0.start = std::min(sv0.start,next->start);
	  sv0.end = std::max(sv0.end,next->end);
		cnvs_list.erase(cnvs_list.begin()+(i+1));
		}
	else
		{
		i++;
		}
	}
/* at the end print everything */
for(auto r=cnvs_list.begin();r!=cnvs_list.end();++r) {
	cout << reg->contig() << "\t" << r->start << "\t"  << r->end;
	cout << "\t" << (r->svtype==1?"DUP":"DEL");
	cout << "\t" << CoordMath::getLength(r->start, r->end);
	cout << "\t" << bam_file ;
	cout << "\t" << (bai_file==NULL?".":bai_file);
	cout << "\t" << median;
	cout << "\t" << average(normalized,TO_BIN(r->start) , TO_BIN(r->end));
	if(ansi_blocks) {
		cout << "\t";
		int d = (int)((r->end - r->start)*1.5);
		int x1 = std::max(0,r->start-d);
		int x2 = std::min((int)normalized.size(),r->end+d);
		while(x1<x2) {
			cout << ansi::ANSI::block(normalized[x1++]);
			}
		}
	cout << endl;
	}
return 0;
}

