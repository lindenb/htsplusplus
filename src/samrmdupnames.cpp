#include "RegIdx.hh"


#include "SamFilterApp.hh"
#include "programs.hh"

using namespace std;

	
static bool sameRead(final SamRecord* rec1, final SamRecord* rec2) {
		if(rec1->isSecondaryOrSupplementary() && rec2->isSecondaryOrSupplementary()) return false;
		int i =  rec1->getFlags() - rec2->getFlags();
		if(i!=0) return false;
		i = strcmp(rec1->getReadName(),rec2->getReadName());
		return i==0;
	}
	 
	
static bool samePos(final SamRecord* rec1, final SamRecord* rec2) {
		final int tid1 = rec1->tid();
		final int tid2 = rec2->tid();
		int i = tid1 - tid2;
		if(i!=0) return false;
		final hts_pos_t p1 = rec1->start();
		final hts_pos_t p2 = rec2->start();
		i =  p1 - p2;
		return i==0;
		}



static long dump(SamFileWriter* w, std::vector<SamRecord*>& buffer) {
		long n_removed = 0L;
		unsigned int x=0;
		while(x +1 < buffer.size()) {
			SamRecord* rec1 = buffer.at(x);
			unsigned int y = x+1;
			while(y < buffer.size()) {
				SamRecord* rec2 = buffer.at(y);
				bool same_name = sameRead(rec1,rec2);
				if(same_name) {
					buffer.erase(std::next(buffer.begin(), y));
					n_removed++;
					}
				else
					{
					y++;
					}
				}
			x++;
			}
		
		
		for(SamRecord* rec2: buffer) {
			w->write2(rec2);
			}
		buffer.clear();
		return n_removed;
        }

int main_samrmdupnames(int argc, char** argv) {
	SamrmdupnamesArgs args;
	if(!args.parse(argc,argv)) return EXIT_FAILURE;
	PROGRAM_COMMON(args);
	DEBUG(args.nthreads);
	std::unique_ptr<HtsThreadPool> thread_pool  =  HtsThreadPool::create(args.nthreads);
	SamFileReaderFactory srf;
	long n_removed = 0L;

	const char* input = args.oneFileOrNull();
	std::unique_ptr<SamFileReader> r=srf.
		threads(thread_pool.get()).
		open(input==NULL?"-":input);
	if(!r->header->is_coordinate_sorted()) {
		THROW_ERROR("input should be coordinate sorted");
		}
	SamFileWriterFactory swf;
	std::unique_ptr<SamFileWriter> w=swf.
		threads(thread_pool.get()).
		compression(args.compression_level).
		format(args.bam_output_format).
		open("-");

	w->write_header(r->header->get());
	std::vector<SamRecord*> pool;
	std::vector<SamRecord*> buffer;
	for(;;) {
		while(buffer.size() >= pool.size()) {
			pool.push_back(new SamRecord);
			}

		SamRecord* rec = pool[buffer.size()];
		if(!r->read2(rec)) {
			rec=NULL;
			}

		/* both reads unmaped, we are in the non-mappend section at the end of the SAM file */
		if(rec!=NULL && rec->getReadUnmappedFlag() &&
			(!rec->getReadPairedFlag() || 
			rec->getMateUnmappedFlag())) {
			n_removed += dump(w.get(),buffer);
			continue;
			} /* end of section for unmapped reads */
		
		
		if(rec==NULL || (!buffer.empty() && !samePos(buffer.at(0),rec))) {
			n_removed += dump(w.get(),buffer);
			if(rec==NULL) break;
			}
		
		buffer.push_back(rec);
		}

	for(unsigned int i=0;i< pool.size();i++) {
		delete pool[i];
		}
	w->close();
	return 0;
	}
