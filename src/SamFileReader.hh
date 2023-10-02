#ifndef SAMFILE_HPP
#define SAMFILE_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "SamRecord.hh"
#include "SamFileHeader.hh"

class SamFile{
	public:
		samFile* fp;
		SamFile(samFile* fp):fp(fp) {
			}
		virtual void close() {
			if(fp!=NULL) sam_close(fp);
			fp=NULL;
			}
		virtual ~SamFile(){
			close();
			}
		virtual samFile* get() {
			return fp;
			}
		};

class SamFileReader {
	public:
		std::unique_ptr<SamFile> fp;
		std::unique_ptr<SamFileHeader> header;

		SamFileReader() {
			}
		virtual ~SamFileReader() {
			}
		bool read1(bam1_t* b) {
			int ret= sam_read1(fp->get(),header->get(), b);
			return ret>=0;
			}
		bool read2(SamRecord* rec) {
			return read1(rec->get());
			}
		std::unique_ptr<SamRecord> read2() {
			std::unique_ptr<SamRecord> rec = std::unique_ptr<SamRecord>(new SamRecord);
			if(!read2(rec.get())) {
				rec.reset();
				}
			return rec;
			}
	};

class SamFileReaderFactory {
	private:
		HtsThreadPool* threadPool;
		std::string reference;
	public:
		SamFileReaderFactory():threadPool(NULL){
			}
		SamFileReaderFactory& threads(HtsThreadPool* threadPool) {
			this->threadPool = threadPool;
			return *this;
			}
		std::unique_ptr<SamFileReader> open(const char* filename) {
        	samFile* fp = sam_open_format(filename, "r", NULL);
		    if (fp== NULL) {
		       THROW_ERROR("Cannot open " << filename);
		    	}
		   
		    if (!!hts_set_opt(fp, CRAM_OPT_DECODE_MD, 0)) {
		        THROW_ERROR("Failed to set CRAM_OPT_DECODE_MD value.");
		   		}
			if(threadPool!=NULL) threadPool->bind(fp);
			
		    std::unique_ptr<SamFileHeader> header= SamFileHeader::read(fp);
		
			SamFileReader* sfr=new SamFileReader;
			sfr->fp.reset(new SamFile(fp));
			sfr->header.swap(header);
			
			
			
			std::unique_ptr<SamFileReader> r(sfr);
			
			return r;
			}
	};



#endif

