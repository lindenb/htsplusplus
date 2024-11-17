#ifndef FAIDX_HH
#define FAIDX_HH
#include <htslib/faidx.h>
#include <memory>
#include "SamSequenceDictionary.hh"
#include "Locatable.hh"
#include "debug.hh"
#include "utils.hh"

namespace htspp {

class Faidx:public Pointer<faidx_t> {
	protected:
	   mutable std::shared_ptr<SamSequenceDictionary> _dict;
		 Faidx(faidx_t *fai);
	public:
		virtual ~Faidx();
		virtual hts_pos_t length(const char *reg) const;
		virtual std::shared_ptr<SamSequenceDictionary> dictionary() const;
		virtual std::unique_ptr<std::string> fetch( const char *c_name, hts_pos_t p_beg_i, hts_pos_t p_end_i) const;
		virtual int nseq() const;
		virtual const char* name(int i) const;
		static std::unique_ptr<Faidx> load(const char* filename);
	};


}

#endif
