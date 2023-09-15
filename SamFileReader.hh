#ifndef SAMFILE_HPP
#define SAMFILE_HPP

#include <cerrno>
#include "debug.hh"
#include "SamFileHeader.hh"

class SamRecord {
	public:
		bam1_t* b;

		SamRecord():b(::bam_init1()) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}
			}
		bam1_t* get() {
			return b;
			}
		virtual ~SamRecord() {
			if(b!=NULL) bam_destroy1(b);
			}
	};

class SamFile{
	public:
		samFile* fp;
		SamFile(samFile* fp):fp(fp) {
			}
		void close() {
			if(fp!=NULL) sam_close(fp);
			fp=NULL;
			}
		virtual ~SamFile(){
			close();
			}
		samFile* get() {
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
			std::unique_ptr<SamRecord> rec = std::unique_ptr<SamRecord>(new SamRecord());
			if(read2(rec->get())) {
				rec.reset();
				}
			return rec;
			}
	};

class SamFileReaderFactory {
	private:
		int _nthreads;
		std::string reference;
	public:
		SamFileReaderFactory(){
			}
		SamFileReaderFactory& nthreads(int nt) {
			this->_nthreads = nt;
			return *this;
			}
		std::unique_ptr<SamFileReader> open(const char* filename) {
        	samFile* fp = sam_open_format(filename, "r", NULL);
		    if (fp== NULL) {
		       THROW_ERROR("Cannot open " << filename);
		    	}

		    if (_nthreads > 0) {
		        ::hts_set_threads(fp, _nthreads);
		        }
		   
		    if (!!hts_set_opt(fp, CRAM_OPT_DECODE_MD, 0)) {
		        THROW_ERROR("Failed to set CRAM_OPT_DECODE_MD value.");
		   		}

		    std::unique_ptr<SamFileHeader> header= SamFileHeader::read(fp);
	
			SamFileReader* sfr=new SamFileReader;
			sfr->fp.reset(new SamFile(fp));
			sfr->header.swap(header);
			
			std::unique_ptr<SamFileReader> r(sfr);
			
			return r;
			}
	};



#endif

