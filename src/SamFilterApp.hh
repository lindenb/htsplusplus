#ifndef SAMFILTER_APP_HH
#define SAMFILTER_APP_HH
#include "programs.hh"


#include "SamFileReader.hh"
#include "SamFileWriter.hh"


template<class T>
class SamFilterApp: public T {
	public:
		SamFilterApp() {
			}
		virtual ~SamFilterApp() {
			}
		
		virtual void process() {
			
			}

		virtual void beforeSam() {
			}
		
		virtual void afterSam() {
			}	
		
		virtual int apply(const char* filein, const char* fileout) {
			std::unique_ptr<HtsThreadPool> thread_pool  =  HtsThreadPool::create(this->nthreads);
			SamFileReaderFactory srf;
			const char* input = this->oneFileOrNull();
			std::unique_ptr<SamFileReader> r=srf.
				threads(thread_pool.get()).
				open(input==NULL?"-":input);
			if(r->header->is_coordinate_sorted()) {
				THROW_ERROR("input should not be coordinate sorted");
				}
			SamFileWriterFactory swf;
			std::unique_ptr<RegIdx> regidx = RegIdx::load_file(this->bed_file);

			std::unique_ptr<SamFileWriter> w=swf.
				threads(thread_pool.get()).
				compression(this->compression_level).
				format(this->bam_output_format).
				open("-");

			w->write_header(r->header->get());
			std::vector<SamRecord*> pool;
			std::vector<SamRecord*> buffer;
			bool in_bed=false;
			for(;;) {
				while(buffer.size() >= pool.size()) {
					pool.push_back(new SamRecord);
					}

				SamRecord* rec = pool[buffer.size()];
				if(!r->read2(rec)) {
					rec=NULL;
					}
				if(rec==NULL ||  (!buffer.empty() && strcmp(buffer[0]->read_name(),rec->read_name())!=0)) {
					if(this->inverse) in_bed=!in_bed;
					if(in_bed) {
						for(unsigned int i=0;i< buffer.size();i++) {
							w->write2(buffer[i]);
							}
						}
					buffer.clear();
					in_bed=false;
					if(rec==NULL) break;
					}
				if(!in_bed && !rec->is_unmapped() ) {
					const char* contig = r->header->tid2name(rec->tid());
					in_bed = regidx->overlaps(contig,rec->start(),rec->end());
					}
				buffer.push_back(rec);
				}

			for(unsigned int i=0;i< pool.size();i++) {
				delete pool[i];
				}
			w->close();
			return 0;
			}
	};

#endif

