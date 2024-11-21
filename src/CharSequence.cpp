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

#include <sstream>
#include "CharSequence.hh"


using namespace htspp;
using namespace std;

class CCCharseq: public CharSequence {
private:
	 std::string str;
public:
    CCCharseq(const CharSequence* delegate):CharSequence(),str(delegate->to_string()) {
        }
    virtual ~CCCharseq() {
        }
    virtual char at(int i) const {
        return str[i];
        }
    virtual CharSequence::size_t size() const {
        return str.size();
        }
    virtual CharSequence::size_t length() const {
        return str.size();
        }
};

class CharSeqDelegate:public CharSequence {
public:
    const CharSequence* delegate;
    CharSeqDelegate(const CharSequence* delegate):delegate(delegate) {
        }
    virtual ~CharSeqDelegate() {
        }
    virtual char at(int i) const {
        return delegate->at(i);
        }
    virtual CharSequence::size_t size() const {
        return delegate->size();
        }
    virtual CharSequence::size_t length() const {
        return this->size();
        }
};

class SubSeqCharSeq:public CharSeqDelegate {
public:
    CharSequence::size_t start;
    CharSequence::size_t end;
    SubSeqCharSeq(const CharSequence* delegate,CharSequence::size_t start,CharSequence::size_t end):CharSeqDelegate(delegate) {
        }
    virtual ~SubSeqCharSeq() {
        }
    virtual char at(int i) const {
        return delegate->at(i+start);
        }
    virtual CharSequence::size_t size() const {
        return end - start;
        }
};

class TRCharSeq:public CharSeqDelegate {
public:
    int( * modifier)(int);
    TRCharSeq(const CharSequence* delegate, int( * modifier)(int)):CharSeqDelegate(delegate),modifier(modifier) {
        }
    virtual ~TRCharSeq() {
        }
    virtual char at(int i) const {
        return modifier(delegate->at(i));
        }
};


CharSequence::CharSequence() {
}
 
CharSequence::~CharSequence() {
}


bool CharSequence::empty() const {
return size()==0;

}

std::ostream& CharSequence::print(std::ostream& out) const {
for(size_t i=0;i< size();i++) out << at(i);
return out;
}


char CharSequence::operator[](int i) const {
    return at(i);
    }

std::size_t CharSequence::size() const {
return length();
}

std::string CharSequence::to_string() const {
std::ostringstream os;
print(os);
return os.str();
}


bool CharSequence::starts_with(const char* s) const {
    size_t i=0L;
    while(s[i]!=0) {
       if(i>=size()) return false;
        if(s[i]!=at(i)) return false;
        ++i;
        }
    return true;
    }


std::unique_ptr<CharSequence> CharSequence::substring(CharSequence::size_t start) const {
    return this->substring(start,size());
    }
    
 std::unique_ptr<CharSequence> CharSequence::clone() const {
    CCCharseq* c=new CCCharseq(this);
     return std::unique_ptr<CharSequence>(c);
    }   
    

std::unique_ptr<CharSequence> CharSequence::substring(CharSequence::size_t start,CharSequence::size_t end ) const {
    return std::unique_ptr<CharSequence>(new SubSeqCharSeq(this,start,end));
    }
   
   
std::unique_ptr<CharSequence> CharSequence::toupper() const {
    return std::unique_ptr<CharSequence>(new TRCharSeq(this,std::toupper));
    }
std::unique_ptr<CharSequence> CharSequence::tolower() const {
    return std::unique_ptr<CharSequence>(new TRCharSeq(this,std::tolower));
    }
       
   
std::vector<std::unique_ptr<CharSequence> > CharSequence::split(char c) const {
    std::vector<std::unique_ptr<CharSequence> > v;
    split(c,v);
    return v;
    }



#define MATCH_C(C) (C==c)

std::vector<std::unique_ptr<CharSequence> >::size_type CharSequence::split(char c, std::vector<std::unique_ptr<CharSequence> >& v) const {
    v.clear();
    CharSequence::size_t prev=0UL,i=0UL, len = this->size();
	//remove trailing
	while(len>0UL && MATCH_C(at(len-1))) {
		len--;
		}

	for(;;) {
		if(i>=len || c==at(i)) {
			v.push_back(substring(prev,i));
			if(i>=len) break;
			prev=i+1;
			}
		i++;
		}
    return v.size();
    }

/*
std::vector<std::string>::size_type CharSplitter::split(char c,std::vector<std::string>& v) const {
	
	return v.size();
	}
*/

#undef MATCH_C


