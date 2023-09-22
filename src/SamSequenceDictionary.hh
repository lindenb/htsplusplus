#ifndef SAM_SEQUENCE_DICT_HH
#define SAM_SEQUENCE_DICT_HH
#include <cstring>
#include <string>
#include <htslib/vcf.h>
#include <htslib/faidx.h>
#include "utils.hh"

class SamSequenceDictionary {

	public:
		static const unsigned int npos = (unsigned int)-1UL;;
		SamSequenceDictionary() {}
		virtual ~SamSequenceDictionary() {}
		virtual unsigned int nseq() const = 0;
		virtual const char* name(unsigned int i) const = 0;
		virtual hts_pos_t length(unsigned int i) const = 0;
		virtual unsigned int tid(const char* str) const {
			for(unsigned int i=0;i< nseq();++i) {
				if(std::strcmp(name(i),str)==0) return i;
				}
			return SamSequenceDictionary::npos;
			}
		virtual unsigned long genome_length() const {
			unsigned long _length = 0UL;
			for(unsigned int i=0;i< nseq();++i) {
				_length += length(i);
				}
			return _length;
			}
		virtual bool equals(const SamSequenceDictionary& cp) const {
			if(&cp==this) return true;
			if(cp.nseq()!=nseq()) return false;
			for(unsigned int i=0;i< nseq();++i) {
				if(length(i)!=cp.length(i)) return false;
				if(strcmp(name(i),cp.name(i))!=0) return false;
				}
			return true;
			}
		virtual bool operator==(const SamSequenceDictionary& cp) const {
			return equals(cp);
			}
		static std::unique_ptr<SamSequenceDictionary> of(bcf_hdr_t *header);
		static std::unique_ptr<SamSequenceDictionary> of(faidx_t *fai);
	};

#endif

