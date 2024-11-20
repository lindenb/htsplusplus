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
