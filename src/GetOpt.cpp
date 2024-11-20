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

#include <unistd.h>
#include <cassert>
#include <getopt.h>
#include "GetOpt.hh"
#include "ANSI.hh"
#include "debug.hh"

using namespace std;
using namespace htspp;

 GetOpt::Option::Option(char c,void* ptr):_ptr(ptr),
 			desc("no description"),
 			flag_required(false),
 			flag_help(false),
 			flag_hidden(false),
 			count_seen(0),
 			_argname("arg"),
 			optc(c) {
 	}
 
 GetOpt::Option::~Option()  {
 }

 void GetOpt::Option::touch()  {
 count_seen++;
 }

 GetOpt::Option& GetOpt::Option::help()  {
 flag_help=true;
 return *this;
 }
 
 GetOpt::Option& GetOpt::Option::required()  {
 flag_required=true;
 return *this;
 }
 
 
  GetOpt::Option& GetOpt::Option::hidden()  {
 flag_hidden=true;
 return *this;
 }
 
 GetOpt::Option& GetOpt::Option::longOpt(const char* s) {
long_opt.assign(s);
return *this;
}


 GetOpt::Option& GetOpt::Option::description(const char* s) {
 desc.assign(s);
 return *this;
 }

 
GetOpt::Option& GetOpt::Option::argName(const char* s) {
_argname.assign(s);
return *this;
}

GetOpt::GetOpt():optind(0) {
}

GetOpt::~GetOpt() {
for(size_t i=0;i< options.size();i++) {
	delete options[i];
	}
}
/**********************************************************************************/
class BoolOpt:public  GetOpt::Option {
public:
  bool* ptr;
	BoolOpt(char c,bool* ptr): GetOpt::Option(c,NULL),ptr(ptr) {
		}
	virtual ~BoolOpt() {}
	virtual bool takes_arg() { return false;}
	virtual void process(const char* arg_or_NULL) {assert(arg_or_NULL==NULL);*ptr=true;}
	virtual void write_default(std::ostream& out) {out << *ptr;}
};


GetOpt::Option& GetOpt::boolOpt(char c,bool* arg) {
	return add(new BoolOpt(c,arg));
	}
/**********************************************************************************/
class In32Opt:public  GetOpt::Option {
public:
  int32_t* ptr;
	In32Opt(char c,int32_t* ptr): GetOpt::Option(c,ptr),ptr(ptr) {
		}
	virtual ~In32Opt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);*ptr=atoi(arg);}
	virtual void write_default(std::ostream& out) {out << *ptr;}
};


GetOpt::Option& GetOpt::intOpt(char c,int32_t* arg) {
	return add(new In32Opt(c,arg));
	}
/**********************************************************************************/

class DoubleOpt:public  GetOpt::Option {
public:
  double* ptr;
	DoubleOpt(char c,double* ptr): GetOpt::Option(c,ptr),ptr(ptr) {
		}
	virtual ~DoubleOpt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);*ptr=atof(arg);}
	virtual void write_default(std::ostream& out) {out << *ptr;}
};


GetOpt::Option& GetOpt::doubleOpt(char c,double* arg) {
	return add(new DoubleOpt(c,arg));
	}

/**********************************************************************************/
class CStringOpt:public  GetOpt::Option {
public:
  char** ptr;
	CStringOpt(char c,char** ptr): GetOpt::Option(c,ptr),ptr(ptr) {
		}
	virtual ~CStringOpt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);*ptr=const_cast<char*>(arg);}
	virtual void write_default(std::ostream& out) {out << (*ptr==NULL?"(NULL)":*ptr);}
};


GetOpt::Option& GetOpt::stringOpt(char c,char** arg) {
	return add(new CStringOpt(c,arg));
	}

/**********************************************************************************/


class CCStringOpt:public  GetOpt::Option {
public:
  std::string* ptr;
	CCStringOpt(char c,std::string* ptr): GetOpt::Option(c,ptr),ptr(ptr) {
		}
	virtual ~CCStringOpt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);ptr->assign(arg);}
	virtual void write_default(std::ostream& out) {out << *ptr;}
};


GetOpt::Option& GetOpt::ccStringOpt(char c,std::string* arg) {
	return add(new CCStringOpt(c,arg));
	}

/**********************************************************************************/



class StringsOpt:public  GetOpt::Option {
public:
  std::vector<std::string>* ptr;
	StringsOpt(char c,std::vector<std::string>* ptr): GetOpt::Option(c,ptr),ptr(ptr) {
		}
	virtual ~StringsOpt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);ptr->push_back(arg);}
	virtual void write_default(std::ostream& out) {}
};


GetOpt::Option& GetOpt::stringsOpt(char c,std::vector<std::string>* arg) {
	return add(new StringsOpt(c,arg));
	}
/**********************************************************************************/


GetOpt::Option&  GetOpt::add(GetOpt::Option* opt) {
	if(opt->optc==0 && opt->long_opt.empty() ) {
		THROW_ERROR("no long/short opt defined");
		}
	for(size_t i=0;i< options.size();i++) {
      if(opt->optc!=0 && options.at(i)->optc==opt->optc) {
       THROW_ERROR("duplicate option " << (char)opt->optc);
      	}
       if(!opt->long_opt.empty() && options.at(i)->long_opt==opt->long_opt) {
     		THROW_ERROR( "duplicate option " << opt->long_opt.c_str());
      	}
      if(opt->_ptr!=0 && options.at(i)->_ptr==opt->_ptr) {
      	THROW_ERROR( "two options share the same ptr");
      	}
      }
  options.push_back(opt);
  return *opt;
	}


void GetOpt::usage(std::ostream& out) {
	for(size_t i=0;i< options.size();i++) {
      GetOpt::Option* opt= options.at(i);
      if(opt->flag_hidden) continue;
      out << " ";
      if(opt->optc!=0) {
      	out << ANSI_GREEN( "-" << (char)opt->optc);
      	if(!opt->long_opt.empty()) out << "|";
      	}
      if(!opt->long_opt.empty()) {
      	out << ANSI_GREEN( "--" << opt->long_opt);
      	}
      if(opt->takes_arg()) out << ANSI_BLUE( " <" << opt->_argname << ">" ) ;  
      out <<  " " << ansi::white << opt->desc << ansi::reset;
      if(opt->flag_required)   out <<  " " << ANSI_YELLOW( "[REQUIRED]");
     	if(opt->takes_arg()) {out<< ". " << ansi::cyan << "[" ; opt->write_default(out)  ; out << "]" << ansi::reset ;}
      out << std::endl;
      }
  out << std::endl;
	}

 int GetOpt::parse(int argc,char** argv) {
   std::string optstring;
   std::vector<struct option> loptions;
   for(size_t i=0;i< options.size();i++) {
      GetOpt::Option* opt= options.at(i);
      if(opt->optc==0 && opt->long_opt.empty()) {
      	THROW_ERROR("option.c=0 && option.longopt.empty()");
      }
      
      if(opt->optc!=0) {
				optstring+= opt->optc;
				if(opt->takes_arg()) optstring+=":";
				}
			
			if(!opt->long_opt.empty()) {
				struct option lo={opt->long_opt.c_str(),opt->takes_arg()?required_argument:no_argument,NULL,opt->optc};
				loptions.push_back(	lo);
				}
		}

	 struct option eof={NULL,0,NULL,0};
	 loptions.push_back(eof);
	 bool flag_help_seen=false;
   int opt;
   int long_index=0;
   /** reinitialize optind */
   ::optind = 1;
   while ((opt = ::getopt_long(argc, argv, optstring.c_str(), loptions.data(),&long_index)) >0) {
   			 if (opt == '?') {
	          cerr << ansi::red << "Unknow option `-" << (char)optopt << "` \n" << ansi::reset;
	          return -1;
	          }
        else if (opt == ':') {
	          cerr << ansi::red << "Option `-" << (char)optopt  <<  "` requires an argument.\n" << ansi::reset;
	          return -1;
	          }
   		 GetOpt::Option* option= NULL;
       for(size_t i=0;i< options.size();i++) {
            if(options.at(i)->optc==opt) {
            	option = options.at(i);
            	break;
            	}
          }

       if(option==NULL) {
 	      	cerr << ansi::red << "Unknow option `-" << (char)opt << "` \n" << ansi::reset;
       		return -1;
       		}
       if(option->flag_help) {
            flag_help_seen = true;
       			}
       option->touch();
       option->process(option->takes_arg()?optarg:NULL);		
       }

      for(size_t i=0;i< options.size() && !flag_help_seen ;i++) {
      GetOpt::Option* opt= options.at(i);
			if(opt->flag_required && opt->count_seen==0) {
				cerr << ANSI_RED("Option `-" << (char)opt->optc << "` was not set \n");
			  return -1;
				}
			}
	this->optind= ::optind;
	for(int i=::optind ; i <argc;++i) {
		files.push_back(argv[i]);
		}

     return 0;
     }

