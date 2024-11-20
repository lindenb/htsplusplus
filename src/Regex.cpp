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

#include "Regex.hh"
#include "debug.hh"
#include "utils.hh"
#include "CharSplitter.hh"
#include "StringUtils.hh"
using namespace htspp;

Regex::Regex() {
    }
                                                                    
Regex::~Regex() {
    ::regfree(&regex);
    }

bool Regex::matches(const char* s) {
    return (::regexec(&regex,s,0,NULL,0)==0);
    }


std::unique_ptr<Regex> Regex::compile(const char* s,int flags) {
	ASSERT_NOT_NULL(s);
	Regex* r = new Regex();
	if(regcomp(&(r->regex),s,flags)!=0) {
		delete r;
		THROW_ERROR("cannot compile regular expression \"" << s << "\".");
		}
	return std::unique_ptr<Regex>(r);
	}

#define REGEXFLAG(F) else if(v[i].compare(#F)==0) {flag|=F;}

int Regex::parse_flags(const char* s) {
	ASSERT_NOT_NULL(s);
	CharSplitter splitter(", ;|\t");
	std::vector<std::string> v=splitter.split(s);
	int flag = 0;
	for(unsigned int i=0;i<v.size();++i) {
		if(StringUtils::isBlank(v[i].c_str())) continue;
		REGEXFLAG(REG_EXTENDED)
		REGEXFLAG(REG_ICASE)
		REGEXFLAG(REG_NOSUB)
		REGEXFLAG(REG_NEWLINE)
		else
			{
			THROW_ERROR("unknow regex flag " << v[i]);
			}
		}
	return flag;
	}
#undef REGEXFLAG
