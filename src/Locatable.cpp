#include "CoordMath.hh"
#include "Locatable.hh"

using namespace htspp;


Locatable::Locatable() {}
Locatable::~Locatable() {}

		 bool Locatable::has_contig(const char* ctg) const {
			return ctg!=NULL && std::strcmp(ctg,contig())==0;
			}
		 bool Locatable::within_distance_of(Locatable* other, hts_pos_t distance) const {
			if(!has_contig(other->contig())) return false;
			return CoordMath::overlaps(start(), end(), other->start()-distance, other->end()+distance);
			}
		 hts_pos_t Locatable::length_on_reference() const {
			return  CoordMath::getLength(start(),end()); 
			}
		
		 std::ostream& Locatable::print(std::ostream& out) const {
			out << contig() << ":" << start() << "-" << end();
			return out;
			}	
			
		 std::string Locatable::to_string() const {
			std::ostringstream os;
			print(os);
			return os.str();
			}
