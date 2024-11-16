#include <cstring>
#include "Interval.hh"
#include "debug.hh"
using namespace std;
using namespace htspp;

 Interval Interval::of(std::string s) {
	string::size_type colon = s.find(':');
	
    if(colon==string::npos) {
        THROW_ERROR("cannot find ':' in " << s);        
        }
    string::size_type hyphen = s.find('-',colon+1);    
    if(hyphen==string::npos) {
        THROW_ERROR("cannot find '-' in " << s);        
        }
    string contig = s.substr(0,colon);
    if(contig.empty()) {
        THROW_ERROR("empty contig in " << s);        
        }
        
     
        
    hts_pos_t beg = std::stoi( s.substr(colon+1,hyphen-(colon+1)));
    if(beg<1)  {
        THROW_ERROR("start < 1 in " << s);        
        }
    hts_pos_t stop = std::stoi( s.substr(hyphen+1));
     if(beg>stop)  {
        THROW_ERROR("start > stop in " << s);        
        }
    Interval p(contig.c_str(),bed,stop);
    return p;    
    }


std::ostream& operator << (std::ostream& out,const Interval& o) {
    out << o.contig() << ":" << o.start() << "-" << o.end();
    return out;    
    }
