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

#ifndef OPTION_VALUES_HH
#define OPTION_VALUES_HH
#include <vector>
#include <memory>
#include "Option.hh"

namespace htspp {
    

template<typename T>
    class OptionValues: public Option {
        public:
            std:vector<T> value;
            OptionValues(int optc,const char* long_opt,const char* description):Option(optc,long_opt,description) {
		}
            virtual ~OptionValues() {
		}
            virtual bool is_flag() {
		return false;
		}
            virtual int process(const char* p) {
		    std::string s(p);
		    std::stringstream ss{s};
		    T t;
		    ss >> t;
		    if(ss.fail()) {
		        cerr << label() << ": Cannot convert "<<s<<" to " << typeid(T).name() << endl;
                        return -1;
        		}
		    this->values.push_back(t);
                    return 0;
		    }
    };

}

#endif
