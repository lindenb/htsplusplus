#ifndef BCF_READER_HH
#define BCF_READER_HH

#include "HtsFile.hh"
#include "BcfHeader.hh"
#include "BcfRecord.hh"

class BcfReader {
	public:
		std::unique_ptr<HtsFile> fp;
		std::unique_ptr<BcfHeader> header;
	private:
		BcfReader() {
			}
	public:
		virtual void close() {
			fp->close();
			}
		virtual  ~BcfReader() {
			close();
			}
		bool read1(BcfRecord* rec) {
			if(rec==NULL) return false;
			for(;;) {
			       int ret= ::bcf_read(this->fp->get(), this->header->get(), rec->get());
		       		if(rec->get()->errcode!=0) {
		            		THROW_ERROR("Error in VCF record at tid "<<rec->bcf->rid<<":"<<(rec->bcf->pos+1));
					continue;
            				}
				return ret==0;
				}
			}
		std::unique_ptr<BcfRecord> read1() {
			std::unique_ptr<BcfRecord> ret(new BcfRecord);
			if(!read1(ret.get())) {
				ret.reset();
				}
			return ret;
			}
		bool read2(BcfRecordHeader* rec) {
			if(rec==NULL) return false;
			bool b = read1(rec);
			rec->header = header->get();
			return b;
			}
		std::unique_ptr<BcfRecordHeader> read2() {
			std::unique_ptr<BcfRecordHeader> ret(new BcfRecordHeader);
			if(!read2(ret.get())) {
				ret.reset();
				}
			return ret;
			}
		static std::unique_ptr<BcfReader> open(const char* filename);
	};


#endif
