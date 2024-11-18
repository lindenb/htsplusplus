#include <iostream>
#include "HtsIdx.hh"
#include "debug.hh"

using namespace std;
using namespace htspp;

HtsIndex::HtsIndex(hts_idx_t *idx,bool free_on_close):Pointer(idx),free_on_close(free_on_close) {
	}

HtsIndex::~HtsIndex() {
  dispose();
  }

void HtsIndex::dispose() {
  if(is_not_null() && this->free_on_close) {
  	::hts_idx_destroy(get());
  	}
  set_null();
  }

std::unique_ptr<HtsIndex> HtsIndex::load( const char* filename,int fmt,int flags) {
  ASSERT_NOT_NULL(filename);
	return HtsIndex::of(::hts_idx_load(filename,fmt));
	}

std::unique_ptr<HtsIndex> HtsIndex::wrap(hts_idx_t* idx) {
	std::unique_ptr<HtsIndex> p(new HtsIndex(idx,false));
	return p;
	}

std::unique_ptr<HtsIndex> HtsIndex::of(hts_idx_t* idx) {
	ASSERT_NOT_NULL(idx);
	std::unique_ptr<HtsIndex> p(new HtsIndex(idx,true));
	return p;
	}

int HtsIndex::nseq() {
	return ::hts_idx_nseq(require_not_null());
}