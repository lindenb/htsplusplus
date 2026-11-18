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

#ifndef OPTION_HH
#define OPTION_HH
#include <string>
#include <cstdio>
#include <cstdlib>
namespace htspp {

    
    class Option {
        public:
            int optc;
            /** long opt or empty */
	        std::string long_opt;
            /** description */
            std::string desc;
            /** visited by getopt */
            int count_seen;
            /** hidden in usage */
            bool flag_hidden;
            /** required */
            bool flag_required;
	    /** argument name when displaying the usage (like 'file', 'url', etc...) */
            std::string arg_name;
        protected:
            Option(int optc,const char* long_opt,const char* description);
        public:
            virtual ~Option();
	    /** option take no argument */
            virtual bool is_flag()=0;
            /** return a name for this option like '-i' or '--inverse' */
            virtual std::string label();
	    virtual int process(const char* s)=0;
    };

}

#endif
