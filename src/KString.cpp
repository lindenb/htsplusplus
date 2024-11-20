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

#include "KString.hh"

using namespace std;
using namespace htspp;

KString::KString():ks(KS_INITIALIZE) {
	}

KString::KString(const char* s):ks(KS_INITIALIZE) {
	::kputs(s,&ks);	
	}

KString::KString(const KString& cp):ks(KS_INITIALIZE) {
	::kputsn(cp.c_str(),cp.size(),&ks);	
	}

KString::~KString() {
	::ks_free(&ks);
	}
	
kstring_t* KString::get() {
	return  &ks;
	}

char KString::at(int i) const {
    return ks.s[i];
    }
char& KString::operator[](int i) {
    return ks.s[i];
    }
const char* KString::c_str() const {
    return ::ks_c_str(const_cast<kstring_t*>(&ks));
    }

std::ostream& KString::print(std::ostream& out) const{
	 out.write(c_str(),size());
   return out;
    }
    
    
KString& KString::reserve(std::size_t n) {
	::ks_resize(&ks,n);
	return *this;
	}


size_t KString::length() const {
	return size();
}

KString& KString::clear() {
	::ks_clear(get());
	return *this;
}


KString& KString::append(const char* s) {
	::kputs(s,&ks);
	return *this;
}

KString& KString::append(char c) {
	::kputc(c,get());
	return *this;
}

KString& KString::append(const char* s,size_t n) {
	::kputsn(s,n,get());
	return *this;
}

std::ostream& operator<<(std::ostream& out, const KString& cp) {
    return cp.print(out);
    }

