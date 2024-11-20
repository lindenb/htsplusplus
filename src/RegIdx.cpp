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

#include "debug.hh"
#include "RegIdx.hh"

using namespace std;


std::unique_ptr<RegIdx> RegIdx::load_file(const char* in_fname) {
	if(in_fname==NULL) {
		THROW_ERROR("filename is NULL");
		}
	RegIdx* ptr = new RegIdx;
	ptr->idx = ::regidx_init(in_fname, NULL , NULL ,0, NULL);
	if(ptr->idx==NULL) {
		delete ptr;
		THROW_ERROR("cannot load " << in_fname);
		}
	return std::unique_ptr<RegIdx>(ptr);
	}
