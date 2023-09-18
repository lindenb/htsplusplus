#ifndef SAM_SEQUENCE_DICT_HH
#define SAM_SEQUENCE_DICT_HH
#include <htslib/vcf.h>
#include "utils.hh"
#include "SamSequenceRecord.hh"

class SamSequenceDictionary: public AbstractList<SamSequenceRecord*> {
	private:
		long length;
	public:
		SamSequenceDictionary():length(-1L) {}
		virtual ~SamSequenceDictionary() {}
		static std::unique_ptr<SamSequenceDictionary> of(bcf_hdr_t *header);
	};

#endif

