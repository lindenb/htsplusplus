#ifndef SAMFILE_HPP
#define SAMFILE_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "SamRecord.hh"
#include "SamFileHeader.hh"
#include "Coverage.hh"

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
		
class SamRecordIterator {
	protected:
		SamRecord* _record;
		samFile* fp;
	public:
		SamRecordIterator(samFile* fp):_record(NULL),fp(fp) {
			_record = new SamRecord;
			}
		virtual ~SamRecordIterator() {
			delete _record;
			}
		virtual bool next()=0;
		virtual void close() {}
		virtual SamRecord* sam() { return _record;}
	};


class SamRecordScanIterator: public SamRecordIterator {
	private:
		sam_hdr_t* header;
	public:
		SamRecordScanIterator(samFile* fp,sam_hdr_t* header):SamRecordIterator(fp),header(header) {
			}
		virtual ~SamRecordScanIterator() {
			}
		virtual void close() {
			header=NULL;
			}
		virtual bool next() {
			if(header==NULL) return false;
			int ret= ::sam_read1(fp,header, this->_record->get());
			return ret>=0;
			}
	};

class SamRecordIndexIterator: public SamRecordIterator {
	private:
		hts_itr_t* iter;
	public:
		SamRecordIndexIterator(samFile* fp,hts_itr_t* iter):SamRecordIterator(fp),iter(iter) {
			}
		virtual ~SamRecordIndexIterator() {
			if(iter!=NULL) ::hts_itr_destroy(iter);
			}
		virtual void close() {
			if(iter!=NULL) ::hts_itr_destroy(iter);
			iter=NULL;
			}
		virtual bool next() {
			if(iter==NULL) return false;
			int ret=::sam_itr_next(this->fp,this->iter,this->_record->get());
			return ret>=0;
			}
	};

class SamFileReader {
	public:
		typedef int (*callback1_t)(SamRecord*,void* data);
		std::unique_ptr<SamFile> fp;
		std::unique_ptr<SamFileHeader> header;
		//associated index
		hts_idx_t *idx;

		SamFileReader():idx(NULL) {
			}
		virtual ~SamFileReader() {
			if(idx!=NULL) ::hts_idx_destroy(idx);
			}
		bool has_index() {
			return idx!=NULL;
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
		std::unique_ptr<SamRecordIterator> query(const Locatable* loc) {
			if(loc==NULL) THROW_ERROR("loc is NULL");
			std::string str = loc->to_string();
			return query(str.c_str()); 
			}
		std::unique_ptr<SamRecordIterator> query(const char* reg) {
			if(reg==NULL) THROW_ERROR("reg is NULL");
			hts_itr_t* iter = ::sam_itr_querys(idx, header->get(), reg);
			if(iter==NULL) THROW_ERROR("cannot sam_itr_querys for "<< reg);
			SamRecordIterator* it = new SamRecordIndexIterator(this->fp->get(),iter);
			std::unique_ptr<SamRecordIterator> p(it);
			return p; 
			}
		std::unique_ptr<SamRecordIterator> iterator() {
			SamRecordIterator* iter = new SamRecordScanIterator(this->fp->get(),this->header->get());
			std::unique_ptr<SamRecordIterator> p(iter);
			return p; 
			}
	};

class SamFileReaderFactory {
	private:
		HtsThreadPool* threadPool;
		std::string reference;
		bool requires_index;
	public:
		SamFileReaderFactory():threadPool(NULL),requires_index(false) {
			}
		SamFileReaderFactory& threads(HtsThreadPool* threadPool) {
			this->threadPool = threadPool;
			return *this;
			}
		SamFileReaderFactory load_index(bool b) {
			this->requires_index = b;
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
			
			hts_idx_t *idx = NULL;
			if(this->requires_index) {
				idx = ::sam_index_load(fp,filename);
				if(idx==NULL) {
					THROW_ERROR("Cannot load index for " << filename);
					}
				}
			
			SamFileReader* sfr=new SamFileReader;
			sfr->fp.reset(new SamFile(fp));
			sfr->header.swap(header);
			
			
			
			std::unique_ptr<SamFileReader> r(sfr);
			
			return r;
			}
	};



#endif

