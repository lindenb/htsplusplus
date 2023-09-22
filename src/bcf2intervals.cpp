#include <string>
#include "programs.hh"
#include "BcfReaderPeeker.hh"

using namespace std;

int main_bcf2intervals(int argc, char** argv) {
Bcf2intervalsArgs args;
args.parse(argc,argv);
PROGRAM_COMMON(args);
const char* input = args.oneFileOrNull();
std::unique_ptr<BcfReader> in0 = BcfReader::open(input==NULL?"-":input);
BcfReaderPeeker in(in0.get());


BcfRecordHeader* bcf;


std::ostream& out = cout;

while((bcf=in.read())!=NULL) {
 	string contig = bcf->contig();
 	hts_pos_t rgn_start = bcf->start();
 	hts_pos_t rgn_end  = bcf->end();
	unsigned long n_variants = 1;


	for(;;) {
		BcfRecordHeader* curr = in.peek();
		if(curr==NULL) break;
		if (!curr->has_contig(contig.c_str()))  {
			break;
			}
		if (CoordMath::getLength(rgn_start, curr->end()) > args.distance_per_interval) {
			break;
			}
		rgn_end  = curr->end();
		in.read();// consumme
		n_variants++;
		}
	// next variant is just too close than the last one
	for(;;) {
		if(args.min_distance < 0) break;
		BcfRecordHeader* curr = in.peek();
		if(curr==NULL || !curr->has_contig(contig.c_str())) break;
		if( CoordMath::getLength(rgn_end,curr->end()) > args.min_distance) break;
		rgn_end = std::max(rgn_end,curr->end());
		in.read();// consumme
		n_variants++;
		}
	out << contig << "\t" << rgn_start << "\t" << rgn_end << "\t" << CoordMath::getLength(rgn_start, rgn_end) << "\t" << n_variants << std::endl;
	}
out.flush();
delete bcf;
return 0;
}
