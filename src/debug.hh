#ifndef DEBUG_HH
#define DEBUG_HH
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "ANSI.hh"

#define WHERE do { std::cerr << ansi::cyan << "[" << __FILE__ << ":" << __LINE__<<"]" << ansi::reset ;} while(0)
#define DEBUG(...) do {std::cerr << ansi::blue << "[DEBUG]" << ansi::reset ;WHERE;std::cerr << __VA_ARGS__ << std::endl;} while(0)
#define INFO(...) do {std::cerr << ansi::yellow << "[INFO]" << ansi::reset ;WHERE;std::cerr << __VA_ARGS__ << std::endl;} while(0)
#define WARNING(...) do {std::cerr << ansi::magenta << "[WARNING]" << ansi::reset ;WHERE;std::cerr << __VA_ARGS__ << std::endl;} while(0)
#define ERROR(...) do {std::cerr << ansi::red << "[ERROR]" << ansi::reset ;WHERE;std::cerr << __VA_ARGS__ << std::endl;abort();} while(0)
#define THROW_ERROR(...) do {std::ostringstream os; os <<  "[" << __FILE__ << ":" << __LINE__ << "]" << __VA_ARGS__; throw std::runtime_error(os.str());} while(0)

#define ASSERT_NOT_NULL(a) if(a==NULL) THROW_ERROR(#a  " is NULL")



#endif
