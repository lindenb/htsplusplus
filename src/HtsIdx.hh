#ifndef HTS_IDX_HH
#define HTS_IDX_HH
#include <memory>
#include <htslib/hts.h>
#include "utils.hh"
class HtsIndex: public Pointer<hts_idx_t> {
	public:
		HtsIndex(hts_idx_t *idx);
		virtual void dispose();
		virtual ~HtsIndex();
		static std::unique_ptr<HtsIndex> load(const char* filename,int fmt,int flags);
};

#endif
