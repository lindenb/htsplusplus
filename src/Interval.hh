#ifndef INTERVAL_HH
#define INTERVAL_HH
#include <string>
#include "Locatable.hh"

class Interval : public Locatable {
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

	};

#endif
