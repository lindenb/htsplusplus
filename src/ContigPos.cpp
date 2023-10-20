#include <cstring>
#include "ContigPos.hh"
#include "debug.hh"

using namespace std;

ContigPos ContigPos::of(std::string s) {
    string::size_type colon = s.find(':');
    if(colon==string::npos) {
        THROW_ERROR("cannot find ':' in " << s);        
        }
    string contig = s.substr(0,colon);
    if(contig.empty()) {
        THROW_ERROR("empty contig in " << s);        
        }
    hts_pos_t pos = std::stoi( s.substr(colon+1));
    ContigPos p(contig.c_str(),pos);
    return p;    
    }


std::ostream& operator << (std::ostream& out,const ContigPos& o) {
    out << o.contig() << ":" << o.position();
    return out;    
    }
