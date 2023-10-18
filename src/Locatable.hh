#ifndef LOCATABLE_HH
#define LOCATABLE_HH
#include <sstream>
#include <iostream>
#include <string>
#include <cstring>
#include <htslib/hts.h>
#include "utils.hh"

class Locatable {
	public:
		Locatable() {}
		virtual ~Locatable() {}
		virtual const char* contig() const =0;
		virtual hts_pos_t start() const =0;
		virtual hts_pos_t end() const =0;
		virtual bool has_contig(const char* ctg) const {
			return ctg!=NULL && std::strcmp(ctg,contig())==0;
			}
		virtual bool within_distance_of(Locatable* other, hts_pos_t distance) const {
			if(!has_contig(other->contig())) return false;
			return CoordMath::overlaps(start(), end(), other->start()-distance, other->end()+distance);
			}
		virtual hts_pos_t length_on_reference() const {
			return  CoordMath::getLength(start(),end()); 
			}
		
		virtual std::ostream& print(std::ostream& out) const {
			out << contig() << ":" << start() << "-" << end();
			return out;
			}	
			
		virtual std::string to_string() const {
			std::ostringstream os;
			print(os);
			return os.str();
			}
	};


#endif
