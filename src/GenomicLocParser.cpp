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

#include <cstring>
#include <memory>
#include <algorithm>
#include "GenomicLocParser.hh"
#include "Interval.hh"
#include "debug.hh"

using namespace htspp;
using namespace std;


class TidStartEnd : public Locatable  {
	private:
		const SamSequenceRecord* ssr;
		hts_pos_t beg;
		hts_pos_t stop;
	public:
		TidStartEnd(const SamSequenceRecord* ssr,hts_pos_t start,hts_pos_t end):ssr(ssr),beg(start),stop(end) {}
		virtual ~TidStartEnd() {}
		virtual const char* contig() const { return ssr->contig();}
		virtual hts_pos_t start() const { return beg;}
		virtual hts_pos_t end() const { return stop;}
	};


GenomicLocParser::GenomicLocParser():dict(NULL),_trim_pos(false),
			_resolve_contig(false),
			_contig_only(false),
			_rm_commas(false),
			_with_suffix(false) {
	}

GenomicLocParser::GenomicLocParser(const SamSequenceDictionary* dict):dict(dict) {
	}


GenomicLocParser::~GenomicLocParser() {
}

GenomicLocParser& GenomicLocParser::resolve_contigs(bool b) {
_resolve_contig = b;
return *this;
}

GenomicLocParser& GenomicLocParser::trim_positions(bool b) {
_trim_pos = b;
return *this;
}

GenomicLocParser& GenomicLocParser::enable_contig_only(bool b) {
_contig_only = b;
return *this;
}

GenomicLocParser& GenomicLocParser::remove_commas(bool b) {
_rm_commas = b;
return *this;
}

GenomicLocParser& GenomicLocParser::enable_suffix(bool b) {
_with_suffix = b;
return *this;
}

#define IF_SUFFIX(suffix,FACTOR) \
	if(s.length() > strlen(suffix) && s.compare(s.length() - strlen(suffix), strlen(suffix), suffix)==0 ) { s.erase(  s.length() - strlen(suffix),strlen(suffix)); factor= FACTOR;}

hts_pos_t GenomicLocParser::parseInt(const char* src,std::string s) const {
	int factor=1;
	if(_rm_commas) {
		auto r_end= std::remove(s.begin(), s.end(), ',');
		s.erase(r_end, s.end());
		}
	if(_with_suffix) {
		IF_SUFFIX("bp",1)
		else IF_SUFFIX("b",1)
		else IF_SUFFIX("kb",1000)
		else IF_SUFFIX("k",1000)
		else IF_SUFFIX("mb",1E6)
		else IF_SUFFIX("m",1E6)
		else IF_SUFFIX("gb",1E9)
		else IF_SUFFIX("g",1E9)
		}
	hts_pos_t p = std::stoi(s);
	p*=factor;
	return p;
	}
#undef IF_SUFFIX

std::unique_ptr<Locatable> GenomicLocParser::parse(const char* s0) const {
	std::unique_ptr<Locatable> p;
	std::string s(s0);
	std::string contig;
	string::size_type colon = s.find(':');
	hts_pos_t beg;
	hts_pos_t stop;
	
    if(colon==string::npos) {
        if(!_contig_only) THROW_ERROR("cannot find ':' in " << s);
        contig.assign(s);
        beg = 1;
        stop = INT_MAX;
        }
    else
    		{
    		string::size_type hyphen = s.find('-',colon+1);    
				if(hyphen==string::npos) {
				    THROW_ERROR("cannot find '-' in " << s);        
				    }
     		contig = s.substr(0,colon);
    		 beg = parseInt(s0, s.substr(colon+1,hyphen-(colon+1)));
    		 if(beg<1 && _trim_pos) {
    		 		beg= 1;
    		 		}
				 else if(beg<1)  {
				    THROW_ERROR("start < 1 in " << s);        
				    }
     		stop = parseInt(s0, s.substr(hyphen+1));
    		}
    		
    if(dict!=NULL && this->_resolve_contig) {
    	std::unique_ptr<std::string> ctg2 = dict->resolve_contig(contig.c_str());
    	if(ctg2.get()==NULL) {
    		 THROW_ERROR("contig is not in dictionary " << s);
    		}
    	contig.assign(*(ctg2.get()));
      }
    
    
    if(contig.empty()) {
        THROW_ERROR("empty contig in " << s);        
        }
     
    const SamSequenceRecord* ssr = (this->dict==NULL?NULL: this->dict->getSequence(contig.c_str()));
    if(this->dict!=NULL && ssr==NULL) {
    	THROW_ERROR("contig is not in dictionary in " << s);
    	}
   
    
    if(_trim_pos && ssr!=NULL && stop > ssr->length()) {
     		stop = std::min( stop, ssr->length() );
     		}    
    
    
     if(beg>stop)  {
        THROW_ERROR("start > stop in " << s);        
        }
        
     if(ssr!=NULL && ssr->end()< beg) {
    	THROW_ERROR("beg is greater than contig length " << s);
    	}
    
    if(ssr!=NULL) {
    	p.reset(new TidStartEnd(ssr,beg,stop));
    	}
    else
    	{
    	p.reset(new Interval(contig.c_str(),bed,stop));
    	}
    return p;    
		}