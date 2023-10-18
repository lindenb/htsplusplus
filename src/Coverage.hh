#ifndef COVERAGE_HH
#define COVERAGE_HH
#include "Interval.hh"



class Coverage : public Interval {
	public:
		unsigned int* cov;
		Coverage(const char* c,hts_pos_t start,hts_pos_t end):Interval(c,start,end) {
			unsigned int  len = end-start;
			cov = new unsigned int[len];
			}
		virtual ~Coverage() {
			delete[] cov;
			}
		unsigned int get(unsigned int idx) {
			if(idx>=length_on_reference()) THROW_ERROR("index out of bound");
			return cov[idx];
			}
		unsigned int get_genome( hts_pos_t idx) {
			if(idx< start()-1) return 0UL;
			if(idx> end()) return 0UL;
			return cov[idx - start()];
			}
	};

#endif
