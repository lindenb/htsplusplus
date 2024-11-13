#ifndef HTS_IDX_HH
#define HTS_IDX_HH
#include <htslib/hts.h>

class HtsIndex {
	public:
		hts_idx_t *idx;
		HtsIndex();
		HtsIndex(hts_idx_t *idx);
		virtual ~HtsIndex();
		virtual void dispose();
		static HtsIndex* load(const char* filename,int fmt,int flags);
};

#endif
