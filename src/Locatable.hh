#ifndef LOCATABLE_HH
#define LOCATABLE_HH
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <htslib/hts.h>
#include "utils.hh"

namespace htspp {

class Locatable {
	public:
		Locatable();
		virtual ~Locatable();
		virtual const char* contig() const =0;
		virtual hts_pos_t start() const =0;
		virtual hts_pos_t end() const =0;
		virtual bool has_contig(const char* ctg) const ;
		virtual bool within_distance_of(Locatable* other, hts_pos_t distance) const;
		virtual hts_pos_t length_on_reference() const;
		virtual std::ostream& print(std::ostream& out) const;
		virtual std::string to_string() const;
	};

}

#endif
