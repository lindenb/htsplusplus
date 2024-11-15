#ifndef SAM_RECORD_ITERATOR_HH
#define SAM_RECORD_ITERATOR_HH

#include "SamRecord.hh"

namespace htspp {

class SamRecordIterator {
	public:
		SamRecordIterator();
		virtual ~SamRecordIterator();
		virtual bool read(bam1_t* b)=0;
        	virtual bool read2(SamRecord* rec);
		std::unique_ptr<SamRecord> read2();
	};

}

#endif

