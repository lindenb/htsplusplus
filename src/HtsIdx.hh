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
