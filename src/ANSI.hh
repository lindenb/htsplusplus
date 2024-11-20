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
