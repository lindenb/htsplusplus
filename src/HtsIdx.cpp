#include "HtsIdx.hh"

HtsIndex::HtsIndex():idx(NULL) {
	
	}

HtsIndex::HtsIndex(hts_idx_t *idx):idx(idx) {
	}

HtsIndex::~HtsIndex() {
  dispose();
  }
void HtsIndex::dispose() {
  if(idx!=NULL) ::hts_idx_destroy(idx);
  idx=NULL;
	}

HtsIndex* HtsIndex::load( const char* filename,int fmt,int flags) {
	hts_idx_t* i = ::hts_idx_load(filename,fmt);
	return i==NULL?NULL:new HtsIndex(i);
	}