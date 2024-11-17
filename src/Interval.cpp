#include <cstring>
#include "Interval.hh"
#include "GenomicLocParser.hh"
#include "debug.hh"
using namespace std;
using namespace htspp;

Interval::Interval(const char* c,hts_pos_t start,hts_pos_t end):ctg(c),beg(start),stop(end) {
			}
Interval::Interval(const Interval& cp):ctg(cp.ctg),beg(cp.beg),stop(cp.stop) {
			}
Interval::Interval(const Locatable* o):ctg(o->contig()),beg(o->start()),stop(o->end()) {
			}
Interval::~Interval() {
			}
const char* Interval::contig() const { return ctg.c_str();}
hts_pos_t Interval::start() const { return beg;}
hts_pos_t Interval::end() const { return stop;}
int Interval::compareTo(const Interval& cp) const {
			int i = std::strcmp(contig(),cp.contig());
			if(i!=0) return i;
			i= start() - cp.start();
			if(i!=0) return i;
			return end() - cp.end();
			}
		


std::ostream& operator << (std::ostream& out,const Interval& o) {
    out << o.contig() << ":" << o.start() << "-" << o.end();
    return out;    
    }
