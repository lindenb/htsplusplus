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
#include <cstring>
#include "debug.hh"
#include "Program.hh"

using namespace htspp;
using namespace std;


Program::Program(const char* name,const char* description):_name(name),_description(description) {
}
Program::~Program() {
 }
 
bool Program::is_hidden() const {
    return false;
    }
 
const char* Program::name() const { return _name.c_str();}
const char* Program::description() const { return _description.c_str();}


void Program::usage(std::ostream& out) {
	out << _name << endl;
	out << "Description: " << _description << endl;
	this->getopt.usage(out);	
	out << endl;
	}

char* Program::oneFileOrNull(int argc,char** argv) {
	switch(argc) {
		case 1: return NULL;
		case 2:
			if(strcmp(argv[1],"-")==0) return NULL;
			return argv[1];
		default:THROW_ERROR("in " << argv[0]<<": expected one or zero argument but got "<< (argc-1));
		}
	}