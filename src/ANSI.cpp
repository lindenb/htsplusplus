#include <unistd.h>
#include <cstdio>
#include "ANSI.hh"

using namespace ansi;
using namespace std;


bool ANSI::_enabled = ::isatty(::fileno(stdout));


bool ANSI::enabled() { return ANSI::_enabled;}
void ANSI::disable() { ANSI::_enabled=false;}
