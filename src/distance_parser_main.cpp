#include "grammars/distance_parser/distance_parser.hh"
#include "grammars/distance_parser/distance_parser.tab.hpp"
#include "grammars/distance_parser/distance_parser.lex.yy.hh"
#include "debug.hh"

DistanceParser::DistanceParser() {
	}

DistanceParser::~DistanceParser() {
	}


hts_pos_t DistanceParser::parse(const char* s) {
	ASSERT_NOT_NULL(s);
	hts_pos_t v;
	YY_BUFFER_STATE buffer_state = distance_parser_scan_string(s);
	distance_parserparse(&v);
	distance_parser_delete_buffer(buffer_state);
	DEBUG(s);
	DEBUG(v);
	return v;
	}
