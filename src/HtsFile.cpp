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

#include <htslib/sam.h>
#include "debug.hh"
#include "HtsFile.hh"

using namespace std;
using namespace htspp;

HtsFile::HtsFile(htsFile *fp,bool b):Pointer(fp),close_in_descructor(b) {
    }

int HtsFile::set_opt(hts_fmt_option opcode,int value) {
	 return ::hts_set_opt(get(), opcode, value);
	}

void HtsFile::close() {
		if(!is_null()) {
			if(close_in_descructor)	::hts_close(get());
			this->ptr = NULL;
			}
		}
bool HtsFile::is_open() {
	return is_not_null();
	}

void HtsFile::fush() {
	if(!is_null()) ::hts_flush(get());
	}
const htsFormat* HtsFile::format() {
	return ::hts_get_format(get());
}

void HtsFile::set_cache_size(int n) {
	hts_set_cache_size(get(),n);
}



HtsFile::~HtsFile() {
	close();
	}

void HtsFile::set_reference(const char *fn_aux) {
		ASSERT_NOT_NULL(fn_aux);
		if(::hts_set_fai_filename(get(),fn_aux)!=0) {
			THROW_ERROR("Cannot set reference");
			}
		}

std::shared_ptr<HtsFile> HtsFile::open(const char* filename,const char* mode,int modifiers) {
   htsFile* fp;
   errno = 0;
   std::shared_ptr<HtsFile> p;
   if((fp = ::hts_open((filename==NULL?"-":filename),mode))==NULL) {
		return p;
		}
	p.reset(new HtsFile(fp,true));
	return p;
	}

std::shared_ptr<HtsFile> HtsFile::open(const char* filename) {
	return open(filename,"rb",THROW_ON_ERROR);
	}

std::shared_ptr<HtsFile> HtsFile::wrap(htsFile* pt) {
	unique_ptr<HtsFile> p(new HtsFile(pt,false));
	return p;
	}


