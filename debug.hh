#ifndef DEBUG_HH
#define DEBUG_HH
#include <iostream>
#include <sstream>
#include <stdexcept>

#define WHERE do { std::cerr << "[" << __FILE__ << ":" << __LINE__;} while(0)
#define DEBUG(...) do {std::cerr << "[DEBUG]";WHERE;std::cerr << __VA_ARGS__ << std::endl;} while(0)
#define INFO(...) do {std::cerr << "[INFO]";WHERE;std::cerr << __VA_ARGS__ << std::endl;} while(0)
#define WARNING(...) do {std::cerr << "[WARNING]";WHERE;std::cerr << __VA_ARGS__ << std::endl;} while(0)
#define ERROR(...) do {std::cerr << "[ERROR]";WHERE;std::cerr << __VA_ARGS__ << std::endl;abort();} while(0)
#define THROW_ERROR(...) do {std::ostringstream os; os << __VA_ARGS__; throw std::runtime_error(os.str());} while(0)

#define ASSERT_NOT_NULL(a) if(a==NULL) THROW_ERROR(#a  " is NULL")
#endif
