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

#ifndef PROGRAM_ONE_PASS_BAM_HH
#define PROGRAM_ONE_PASS_BAM_HH
#include "Program.hh"
#include "OptionValue.hh"

namespace htspp {

/* a program that read a sam and write a sam */
class ProgramOnePassBam : public Program {
    protected:
        std::shared_ptr<OptionValue<std::string>> reference;
        std::shared_ptr<OptionValue<std::string>> file_out;
    public:
        ProgramOnePassBam(const char* short_name,const char* description);
    
        virtual ~ProgramOnePassBam();
        virtual int main(int argc,char** argv);
    };
};

#endif