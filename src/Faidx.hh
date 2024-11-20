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
