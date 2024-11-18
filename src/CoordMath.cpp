#include "CoordMath.hh"

using namespace htspp;


hts_pos_t CoordMath::getLength(hts_pos_t start, hts_pos_t end) {
	 return (end - start) + 1;
	 }
	 
bool CoordMath::overlaps(hts_pos_t start, hts_pos_t end, hts_pos_t start2,hts_pos_t end2) {
	  return (start2 >= start && start2 <= end) || (end2 >=start && end2 <= end) ||
                encloses(start2, end2, start, end);
	}
	

bool CoordMath::encloses(hts_pos_t outerStart,hts_pos_t outerEnd, hts_pos_t innerStart,hts_pos_t innerEnd) {
        return innerStart >= outerStart && innerEnd <= outerEnd;
    }