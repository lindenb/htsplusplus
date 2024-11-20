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

#include <unistd.h>
#include <cstdio>
#include "ANSI.hh"

using namespace ansi;
using namespace std;


//   https://en.wikipedia.org/wiki/Block_Elements
// LOWER ONE EIGHTH BLOCK … FULL BLOCK
#define BLOCK_CHARS_LEN 9
static const char *const BLOCK_CHARS[BLOCK_CHARS_LEN] = {".","\u2581", "\u2582", "\u2583", "\u2584", "\u2585", "\u2586", "\u2587", "\u2588"};

bool ANSI::_enabled = ::isatty(::fileno(stdout));


bool ANSI::enabled() { return ANSI::_enabled;}
void ANSI::disable() { ANSI::_enabled=false;}

const char* ANSI::block(double fraction) {
	int i = std::min(std::max(0,(int)(fraction*BLOCK_CHARS_LEN)),BLOCK_CHARS_LEN-1) ;
	if(fraction>0 && i==0) i=1;
	return BLOCK_CHARS[i];
	}
