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

#ifndef GETOPT_HH
#define GETOPT_HH
#include <string>
#include <vector>
#include <iostream>
namespace htspp {

class GetOpt {
public:

	class Option {
	public:
	  void* _ptr;
	  std::string desc;
	  std::string long_opt;
	  bool flag_required;
	  bool flag_help;
	  bool flag_hidden;
	  int count_seen;
	  std::string _argname;
	  char optc;
	  Option(char c,void* ptr);
	  virtual ~Option();
		Option& required();
		Option& help();
		Option& hidden();
		Option& description(const char* s);
		Option& argName(const char* s);
		Option& longOpt(const char*);
		virtual void touch();
		virtual void process(const char* arg_or_NULL)=0;
		virtual bool takes_arg()=0;
		virtual void write_default(std::ostream& out)=0;
	};

private:
std::vector<GetOpt::Option*> options;
GetOpt::Option& add(GetOpt::Option* opt);
public:
  std::vector<char*> files;
  int optind;
  
	GetOpt();
	~GetOpt();
	void usage(std::ostream& out);
	Option& boolOpt(char c,bool* arg);
 	Option& intOpt(char c,int32_t* arg);
  Option& doubleOpt(char c,double* arg);
 	Option& stringOpt(char c,char** arg);
 	Option& ccStringOpt(char c,std::string* arg);
 	Option& stringsOpt(char c,std::vector<std::string>* arg);
 	int parse(int argc,char** argv);
};


}





#endif
