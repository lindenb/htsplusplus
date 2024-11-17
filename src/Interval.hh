#ifndef INTERVAL_HH
#define INTERVAL_HH
#include <string>
#include "Locatable.hh"
#include "Comparable.hh"
namespace htspp {

class Interval : public Locatable, Comparable<Interval> {
	private:
		std::string ctg;
		hts_pos_t beg;
		hts_pos_t stop;
	public:
		Interval(const char* c,hts_pos_t start,hts_pos_t end);
		Interval(const Interval& cp);
		Interval(const Locatable* o);
		virtual ~Interval();
		virtual const char* contig() const;
		virtual hts_pos_t start() const;
		virtual hts_pos_t end() const;
		virtual int compareTo(const Interval& cp) const;
		MAKE_DEFAULT_COMPARABLE_METHODS(Interval)
	};

std::ostream& operator << (std::ostream& out,const Interval& o);
}

#endif
