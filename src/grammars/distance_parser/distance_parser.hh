#ifndef DISTANCE_PARSER_HH
#define DISTANCE_PARSER_HH
#include <htslib/hts.h>

struct DistanceParserYYParam {
	};

class DistanceParser {
	public:
		DistanceParser();
		~DistanceParser();
		hts_pos_t parse(const char* s);
	};

#endif

