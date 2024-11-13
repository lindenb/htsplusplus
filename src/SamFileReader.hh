#ifndef SAMFILE_HPP
#define SAMFILE_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "SamRecord.hh"
#include "SamFileHeader.hh"
#include "Coverage.hh"
#include "HtsIdx.hh"

class SamFile {
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
		HtsFile* fp;
		SamFileHeader* header;
		//associated index
		HtsIndex* idx;
		SamFileReader();
		virtual ~SamFileReader();
		bool has_index();
		bool read1(bam1_t* b);
		bool read2(SamRecord* rec);
		std::unique_ptr<SamRecord> read2();
		std::unique_ptr<SamRecordIterator> query(const Locatable* loc);
		std::unique_ptr<SamRecordIterator> query(const char* reg);
		std::unique_ptr<SamRecordIterator> iterator();
	};


#endif

