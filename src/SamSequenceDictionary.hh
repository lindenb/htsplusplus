#ifndef SAM_SEQUENCE_DICT_HH
#define SAM_SEQUENCE_DICT_HH
#include <cstring>
#include <string>
#include <htslib/vcf.h>
#include <htslib/faidx.h>
#include <htslib/sam.h>
#include "utils.hh"

namespace htspp {

class SamSequenceDictionary {

	public:
		static const unsigned int npos = (unsigned int)-1UL;;
		SamSequenceDictionary();
		virtual ~SamSequenceDictionary();
		virtual int32_t nseq() const = 0;
		virtual const char* name(int32_t i) const = 0;
		virtual long length(int32_t i) const = 0;
		virtual int32_t tid(const char* str) const = 0;
		virtual long genome_length() const = 0;
		virtual bool equals(const SamSequenceDictionary& cp) const;
		virtual bool operator==(const SamSequenceDictionary& cp) const;
		
		static std::unique_ptr<SamSequenceDictionary> of_sam_header(sam_hdr_t *header);
		static std::unique_ptr<SamSequenceDictionary> of_bcf_header(bcf_hdr_t *header);
		static std::unique_ptr<SamSequenceDictionary> of_faidx(faidx_t *fai);
	};

}

#endif

