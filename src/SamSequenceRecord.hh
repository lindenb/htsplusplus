#ifndef SAM_SEQUENCE_RECORD_HH
#define SAM_SEQUENCE_RECORD_HH
#include <htslib/hts.h>

class SamSequenceRecord {
	public:
		SamSequenceRecord() {}
		virtual ~SamSequenceRecord() {}
		virtual const char* name() =0;
		virtual hts_pos_t length() const = 0;
		hts_pos_t start() const { return 1;}
		hts_pos_t end() const { return length();}
		
	};

#endif

