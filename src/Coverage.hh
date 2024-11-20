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

#ifndef COVERAGE_HH
#define COVERAGE_HH
#include "Interval.hh"

namespace htspp {

class Coverage : public Interval {
	public:
		unsigned int* cov;
		Coverage(const char* c,hts_pos_t start,hts_pos_t end):Interval(c,start,end) {
			unsigned int  len = end-start;
			cov = new unsigned int[len];
			}
		virtual ~Coverage() {
			delete[] cov;
			}
		unsigned int get(unsigned int idx) {
			if(idx>=length_on_reference()) THROW_ERROR("index out of bound");
			return cov[idx];
			}
		unsigned int get_genome( hts_pos_t idx) {
			if(idx< start()-1) return 0UL;
			if(idx> end()) return 0UL;
			return cov[idx - start()];
			}
	};

}

#endif
