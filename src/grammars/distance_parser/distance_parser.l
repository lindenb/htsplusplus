
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
#include "string"
#include "set"
#include "debug.hh"
#include "distance_parser.hh"
#include "distance_parser.tab.hpp"

#define yylval distance_parserlval

%}


%option noyywrap
%option caseless
%option nounput
%option noinput
%%


kb { yylval.l = 1000L; return LEX_FACTOR;}
mb { yylval.l = 1000000L; return LEX_FACTOR;}
gb { yylval.l = 1000000000L; return LEX_FACTOR;}
[0-9]+([0-9,]+) {
		int x1 =0, x2=0;
		while(x1 < yyleng ) {
			if(yytext[x1]==',') {
				x1++;
				}
			else
				{
				yytext[x2] = yytext[x1];
				x1++;
				x2++;
				}
			}
		yytext[x2]=0;
		try {
			yylval.l = std::stol(yytext);	
			}
		catch(std::exception& err) {
			THROW_ERROR("cannot parse number :" << yytext<< " " << err.what());
			}
		catch(...) {
			THROW_ERROR("cannot parse number :" << yytext);
			}
		return LEX_INT;
		}
[ \t\n]+ return WS;
.	{
	THROW_ERROR("[lex]Error char \"" << yytext << "\".");
	exit(EXIT_FAILURE);
	}

%%

