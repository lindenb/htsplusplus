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

#include "CharSplitter.hh"

using namespace htspp;
using namespace std;

CharSplitter CharSplitter::TAB('\t');
CharSplitter CharSplitter::COMMA(',');

bool CharSplitter::match(char c) const {
	return  delims.find(c)!=std::string::npos ;
	}

CharSplitter::CharSplitter(char delim):delims() {
	delims+=delim;
	}
CharSplitter::CharSplitter(std::string delims):delims(delims) {
	}

std::vector<std::string>::size_type CharSplitter::split(std::string s,std::vector<std::string>& v,int limit) const {
	std::string::size_type prev=0UL,i=0UL, len = s.size();
	//remove trailing
	while(len>0UL && match(s[len-1])) {
		len--;
		}
	if(limit==1) {
		v.push_back(s.substr(0,len));
		return v.size();
		}
	for(;;) {
		if(i>=len || match(s[i])) {
			v.push_back(s.substr(prev,i-prev));
			if(i>=len) break;
			prev=i+1;
			if(limit>1 && v.size()+1==(unsigned int)limit) {
				v.push_back(s.substr(prev,len-prev));
				return v.size();
				}
			}
		i++;
		}
	return v.size();
	}

std::vector<std::string>::size_type CharSplitter::split(std::string s,std::vector<std::string>& v) const {
	return split(s,v,-1);
	}

std::vector<std::string> CharSplitter::split(std::string s) const {
	std::vector<std::string>  v;
	split(s,v);
	return v;
	}
std::vector<std::string> CharSplitter::split(std::string s,int limit) const {
	std::vector<std::string>  v;
	split(s,v,limit);
	return v;
	}