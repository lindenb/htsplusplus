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

#include <cctype>
#include <cstring>
#include "StringUtils.hh"

using namespace htspp;
using namespace std;

bool StringUtils::isBlank(const char* s) {
		if(s==NULL) return true;
		char* p=(char*)s;
		while(*p!=0) {
		  if(isspace(*p)) return true;
			}
		return true;
		}
bool StringUtils::startsWith(const char* s,const char* prefix) {
	return strncmp(s,prefix,strlen(prefix))==0;
	}
bool StringUtils::endsWith(const char* s,const char* suffix) {
  size_t i=strlen(s);
  size_t j=strlen(suffix);
  return i>j && strcmp(&s[i-j],suffix)==0;
	}
