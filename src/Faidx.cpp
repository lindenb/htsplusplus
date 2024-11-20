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

#include "Faidx.hh"

using namespace std;
using namespace htspp;

Faidx::Faidx(faidx_t *fai):Pointer(fai) {
		}

Faidx::~Faidx() {
	if(get()!=NULL) ::fai_destroy(get());
	}

std::unique_ptr<Faidx> Faidx::load(const char* filename) {
	ASSERT_NOT_NULL(filename);
	faidx_t *f = fai_load(filename);
	ASSERT_NOT_NULL(f);
	return std::unique_ptr<Faidx>(new Faidx(f));
	}
	
	int Faidx::nseq() const {
			return ::faidx_nseq(get());
			}
	const char* Faidx::name(int i) const {
		return ::faidx_iseq(get(),i);
		}
  hts_pos_t Faidx::length(const char *reg) const {
	return ::faidx_seq_len64(get(),reg);
	}


std::shared_ptr<SamSequenceDictionary> Faidx::dictionary()  const {
	if(_dict.get()==NULL) {
		_dict.reset( SamSequenceDictionary::of_faidx(get()).release());
	}
	return _dict;
}

std::unique_ptr<std::string> Faidx::fetch( const char *c_name, hts_pos_t p_beg_i, hts_pos_t p_end_i) const {
			ASSERT_NOT_NULL(c_name);
			hts_pos_t len=0;
			char* s = ::faidx_fetch_qual64(get(),c_name,p_beg_i,p_end_i,&len);
			if(s==NULL) {
				if(len==-2) {
					//seq not present
					return std::unique_ptr<std::string>();
					}
				else
					{
					THROW_ERROR("cannot fecth");
					}
				}
			std::string* str=new std::string(s,(std::string::size_type)len);
			std::free(s);
			return std::unique_ptr<std::string>(str);
			}