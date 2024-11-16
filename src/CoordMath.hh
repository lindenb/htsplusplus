#ifndef COORD_MATH_HH
#define COORD_MATH_HH
#include <htslib/hts.h>

namespace htspp {
class CoordMath {
	public:
		static hts_pos_t getLength(hts_pos_t start, hts_pos_t end);
		static bool overlaps(hts_pos_t start, hts_pos_t end, hts_pos_t start2,hts_pos_t end2);
	};

}

#endif
