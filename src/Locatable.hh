#ifndef LOCATABLE_HH
#define LOCATABLE_HH
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
	};


#endif
