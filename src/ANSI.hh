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

#ifndef HTSPP_ANSI_HH
#define HTSPP_ANSI_HH
#include <iostream>

namespace ansi {

class ANSI {
	private:
		static bool _enabled;
	public:
		static bool enabled();
		static void disable();
		static const char* block(double fraction);
	};


#define DEFINE_ANSI_IOMANIP(NAME,OPCODE) \
template < class C, class T > \
std::basic_ostream< C, T > & NAME( std::basic_ostream< C, T > &os ) \
  	{\
  	if(ansi::ANSI::enabled()) \
  		{\
	    os << "\033[" << (OPCODE) << "m";\
	    }\
		return os;\
  	}

 #define DEFINE_ANSI_FG_IOMANIP(NAME,V) \
 	DEFINE_ANSI_IOMANIP(NAME,V) \
 	DEFINE_ANSI_IOMANIP(foreground_##NAME,(V+10)) \
 	DEFINE_ANSI_IOMANIP(bright_##NAME,(V+60)) \
 	DEFINE_ANSI_IOMANIP(foreground_bright_##NAME,(V+70))

// https://en.wikipedia.org/wiki/ANSI_escape_code
DEFINE_ANSI_IOMANIP(reset,0)
DEFINE_ANSI_FG_IOMANIP(black,30)
DEFINE_ANSI_FG_IOMANIP(red,31)
DEFINE_ANSI_FG_IOMANIP(green,32)
DEFINE_ANSI_FG_IOMANIP(yellow,33)
DEFINE_ANSI_FG_IOMANIP(blue,34)
DEFINE_ANSI_FG_IOMANIP(magenta,35)
DEFINE_ANSI_FG_IOMANIP(cyan,36)
DEFINE_ANSI_FG_IOMANIP(white,37)


#undef ANSI_ESCAPE
#undef DEFINE_ANSI_IOMANIP

#define ANSI_RED(a) ansi::red << a  << ansi::reset
#define ANSI_BLUE(a) ansi::blue << a  << ansi::reset
#define ANSI_GREEN(a) ansi::green << a  << ansi::reset
#define ANSI_YELLOW(a) ansi::yellow << a  << ansi::reset
#define ANSI_CYAN(a) ansi::cyan << a  << ansi::reset
#define ANSI_MAGENTA(a) ansi::magenta << a  << ansi::reset
}

#endif
