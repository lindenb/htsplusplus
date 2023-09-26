%{
/*
The MIT License (MIT)

Copyright (c) 2020 Pierre Lindenbaum

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

#include <cassert>
#include <cerrno>
#include <set>
#include <limits>
#include <string>
#include <htslib/hts.h>
#include "debug.hh"
#include "distance_parser.hh"
#include "distance_parser.lex.yy.hh"

using namespace std;

void yyerror (hts_pos_t* v,char const *s) {
   THROW_ERROR("[scanner]" << s << ".");
}


%}



%parse-param {hts_pos_t* shuttle}

%define api.prefix {distance_parser}
%locations


%union {
    long l;
    };

%token WS
%token<l> LEX_INT
%token<l> LEX_FACTOR
%type<l> factor
%%

input: wss LEX_INT factor wss {
	long v  = $2 * $3;
	if(v < 0) {
		THROW_ERROR("hts_pos_t '" << v << "' cannot be negative.");
		}
	if(v > numeric_limits<hts_pos_t>::max()) {
		THROW_ERROR("hts_pos_t '" << v << "' cannot be  greater than '" <<  numeric_limits<hts_pos_t>::max() << "'.");
		}
	*shuttle = v;
	};

factor: {$$=1L;} | LEX_FACTOR {$$=$1;};
wss: | ws ;
ws: WS ;


%%

