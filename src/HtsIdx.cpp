#include <iostream>
#include "HtsIdx.hh"

using namespace std;

HtsIndex::HtsIndex(hts_idx_t *idx):Pointer(idx) {
	}

HtsIndex::~HtsIndex() {
  dispose();
  }

void HtsIndex::dispose() {
  if(is_not_null()) ::hts_idx_destroy(get());
    set_null();
  }

std::unique_ptr<HtsIndex> HtsIndex::load( const char* filename,int fmt,int flags) {
	hts_idx_t* i = ::hts_idx_load(filename,fmt);
	std::unique_ptr<HtsIndex> p( i==NULL?NULL:new HtsIndex(i));
	return p;
	}
