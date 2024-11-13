#include "SamFileReader.hh"




SamFileReader::SamFileReader():idx(NULL) {
			}
		
		SamFileReader::~SamFileReader() {
			if(idx!=NULL) delete idx;
			}
		bool SamFileReader::has_index() {
			return idx!=NULL;
			}
		bool SamFileReader::read1(bam1_t* b) {
			int ret= sam_read1(fp->get(),header->get(), b);
			return ret>=0;
			}
		bool SamFileReader::read2(SamRecord* rec) {
			return read1(rec->get());
			}
		std::unique_ptr<SamRecord> SamFileReader::read2() {
			std::unique_ptr<SamRecord> rec = std::unique_ptr<SamRecord>(new SamRecord);
			if(!read2(rec.get())) {
				rec.reset();
				}
			return rec;
			}
		std::unique_ptr<SamRecordIterator> SamFileReader::query(const Locatable* loc) {
			if(loc==NULL) THROW_ERROR("loc is NULL");
			std::string str = loc->to_string();
			return query(str.c_str()); 
			}
		std::unique_ptr<SamRecordIterator> SamFileReader::query(const char* reg) {
			if(reg==NULL) THROW_ERROR("reg is NULL");
			hts_itr_t* iter = ::sam_itr_querys(idx->idx, header->get(), reg);
			if(iter==NULL) THROW_ERROR("cannot sam_itr_querys for "<< reg);
			SamRecordIterator* it = new SamRecordIndexIterator(this->fp->get(),iter);
			std::unique_ptr<SamRecordIterator> p(it);
			return p; 
			}
		std::unique_ptr<SamRecordIterator> SamFileReader::iterator() {
			SamRecordIterator* iter = new SamRecordScanIterator(this->fp->get(),this->header->get());
			std::unique_ptr<SamRecordIterator> p(iter);
			return p; 
			}