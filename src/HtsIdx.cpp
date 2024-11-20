/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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