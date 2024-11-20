/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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
