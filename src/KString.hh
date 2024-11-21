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

#ifndef KSTRING_HH
#define KSTRING_HH
#include <htslib/hts.h>
#include <iostream>
#include "CharSequence.hh"

namespace htspp {

class KString : public CharSequence {
	public:
		kstring_t ks;
		KString();
		KString(const char* s);
		KString(const KString& cp);
		virtual ~KString();
		virtual kstring_t* get();
		virtual char at(int i) const;
		virtual char& operator[](int i);
		virtual const char* c_str() const;
		virtual KString& reserve(std::size_t n);
		virtual std::ostream& print(std::ostream& out) const;
		
		

		virtual std::size_t length() const;
		virtual std::size_t size() const;
		virtual KString& clear();
		virtual KString& append(const char* s);
		virtual KString& append(char c);
		virtual KString& append(const char* s,std::size_t n);
		
};

std::ostream& operator<<(std::ostream& out, htspp::KString const& cp);

}
#endif

