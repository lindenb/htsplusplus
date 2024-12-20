#include <memory>
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


GenomicLocParser::GenomicLocParser():dict(NULL),_trim_pos(false),_resolve_contig(false),_contig_only(false) {
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
    		 beg = std::stoi( s.substr(colon+1,hyphen-(colon+1)));
    		 if(beg<1 && _trim_pos) {
    		 		beg= 1;
    		 		}
				 else if(beg<1)  {
				    THROW_ERROR("start < 1 in " << s);        
				    }
     		stop = std::stoi( s.substr(hyphen+1));
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