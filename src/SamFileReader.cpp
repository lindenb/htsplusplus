#include "SamFileReader.hh"

using namespace htspp;
using namespace std;

class AbstractSamRecordIter : public SamRecordIterator {
	protected:
		SamFileReader* owner;
		AbstractSamRecordIter(SamFileReader* owner):owner(owner) {
			assert(owner!=NULL);
		  }
	public:
		virtual ~AbstractSamRecordIter() {
			owner=NULL;
			}
  virtual bool read(bam1_t* b)=0;
  	
  virtual bool read2(SamRecord* rec) {
      assert(rec!=NULL);
	    bool b= this->read(rec->get());
      rec->header = (b?owner->header.get():NULL);
      return b;
    	}

	virtual std::unique_ptr<SamRecord> read2() {
    std::unique_ptr<SamRecord> rec = std::unique_ptr<SamRecord>(new SamRecord(owner->header.get()));
    if(!read2(rec.get())) {
        rec.reset();
        }
		return rec;
    }

};


class SamRecordScanIterator: public AbstractSamRecordIter {
	public:
		SamRecordScanIterator(SamFileReader* owner):AbstractSamRecordIter(owner) {
			}
		virtual ~SamRecordScanIterator() {
			}

		virtual bool read(bam1_t* b) {
			if(owner==NULL) return false;
			int ret= ::sam_read1(owner->fp->get(),owner->header->get(), b);
			return ret>=0;
			}
	};

class SamRecordIndexIterator: public AbstractSamRecordIter {
	private:
		hts_itr_t* iter;
	public:
		SamRecordIndexIterator(SamFileReader* owner,hts_itr_t* iter):AbstractSamRecordIter(owner),iter(iter) {
			}
		virtual ~SamRecordIndexIterator() {
			if(iter!=NULL) ::hts_itr_destroy(iter);
			iter=NULL;
			owner=NULL;
			}
		virtual bool read(bam1_t* b) {
			if(iter==NULL) return false;
			int ret=::sam_itr_next(owner->fp->get(),this->iter,b);
			return ret>=0;
			}
	};



		SamFileReader::SamFileReader() {
			}
		
		SamFileReader::~SamFileReader() {
			}
		bool SamFileReader::has_index() {
			return idx!=NULL;
			}
		bool SamFileReader::read1(bam1_t* b) {
			int ret= sam_read1(fp->get(),header->get(), b);
			return ret>=0;
			}
		bool SamFileReader::read2(SamRecord* rec) {
			bool b= read1(rec->get());
			rec->header = header.get();
			return b;
			}

		std::unique_ptr<SamRecord> SamFileReader::read2() {
			std::unique_ptr<SamRecord> rec = std::unique_ptr<SamRecord>(new SamRecord(header.get()));
			if(!read2(rec.get())) {
				rec.reset();
				}
			return rec;
			}
		std::unique_ptr<SamRecordIterator> SamFileReader::query(const Locatable& loc) {
			std::string str = loc.to_string();
			return query(str.c_str());
			}

		std::unique_ptr<SamRecordIterator> SamFileReader::query(const char* reg) {
			if(reg==NULL) THROW_ERROR("reg is NULL");
			hts_itr_t* iter = ::sam_itr_querys(idx->get(), header->get(), reg);
			if(iter==NULL) THROW_ERROR("cannot sam_itr_querys for "<< reg);
			SamRecordIterator* it = new SamRecordIndexIterator(this,iter);
			std::unique_ptr<SamRecordIterator> p(it);
			return p;
			}

		std::unique_ptr<SamRecordIterator> SamFileReader::iterator() {
			SamRecordIterator* iter = new SamRecordScanIterator(this);
			std::unique_ptr<SamRecordIterator> p(iter);
			return p;
			}
