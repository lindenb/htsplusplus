#ifndef HTS_IDX_HH
#define HTS_IDX_HH
#include <memory>
#include <htslib/hts.h>
#include "utils.hh"

namespace htspp {

class HtsIndex: public Pointer<hts_idx_t> {
  private:
  	bool free_on_close;
  	HtsIndex(hts_idx_t *idx,bool free_on_close);
	public:
		virtual void dispose();
		virtual ~HtsIndex();
		virtual int nseq();
		static std::unique_ptr<HtsIndex> load(const char* filename,int fmt,int flags);
	  static std::unique_ptr<HtsIndex> wrap(hts_idx_t* idx);
	  static std::unique_ptr<HtsIndex> of(hts_idx_t* idx);
	};

}

#endif
