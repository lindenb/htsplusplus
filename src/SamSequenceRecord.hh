#ifndef SAM_SEQ_RECORD_HH
#define SAM_SEQ_RECORD_HH

#include "Locatable.hh"

namespace htspp {

class SamSequenceRecord : public Locatable {
	protected:
			SamSequenceRecord();
	public:
			virtual ~SamSequenceRecord();
			virtual const char* name() const =0;
			virtual int32_t tid() const =0;
			virtual hts_pos_t length() const=0;
			virtual hts_pos_t start() const;
			virtual hts_pos_t end() const;
	};

}

#endif
