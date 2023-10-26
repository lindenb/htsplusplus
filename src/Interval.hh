#ifndef INTERVAL_HH
#define INTERVAL_HH
#include <string>
#include "Locatable.hh"
#include "Comparable.hh"
class Interval : public Locatable, Comparable<Interval> {
	private:
		std::string ctg;
		hts_pos_t beg;
		hts_pos_t stop;
	public:
		Interval(const char* c,hts_pos_t start,hts_pos_t end):ctg(c),beg(start),stop(end) {
			}
		Interval(const Interval& cp):ctg(cp.ctg),beg(cp.beg),stop(cp.stop) {
			}
		Interval(const Locatable* o):ctg(o->contig()),beg(o->start()),stop(o->end()) {
			}
		virtual ~Interval() {
			}
		virtual const char* contig() const { return ctg.c_str();}
		virtual hts_pos_t start() const { return beg;}
		virtual hts_pos_t end() const { return stop;}
		virtual int compareTo(const Interval& cp) const {
			int i = std::strcmp(contig(),cp.contig());
			if(i!=0) return i;
			i= start() - cp.start();
			if(i!=0) return i;
			return end() - cp.end();
			}
		
		
		MAKE_DEFAULT_COMPARABLE_METHODS(Interval)

		static Interval of(std::string s);
	};

std::ostream& operator << (std::ostream& out,const Interval& o);

#endif
