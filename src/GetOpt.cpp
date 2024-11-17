#include <unistd.h>
#include <cassert>
#include <getopt.h>
#include "GetOpt.hh"

using namespace std;
using namespace htspp;

 GetOpt::Option::Option(char c,const char* desc):count_seen(0),flag_required(false),flag_help(false),optc(c),desc(desc==NULL?"no description":desc) {
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
 
 GetOpt::Option& GetOpt::Option::longOpt(const char* s) {
long_opt.assign(s);
return *this;
}

GetOpt::GetOpt() {
}

GetOpt::~GetOpt() {
for(size_t i=0;i< options.size();i++) {
	delete options[i];
	}
}

class BoolOpt:public  GetOpt::Option {
public:
  bool* ptr;
	BoolOpt(char c,const char* desc,bool* ptr): GetOpt::Option(c,desc),ptr(ptr) {
		}
	virtual ~BoolOpt() {}
	virtual bool takes_arg() { return false;}
	virtual void process(const char* arg_or_NULL) {assert(arg_or_NULL==NULL);*ptr=true;}
};


GetOpt::Option& GetOpt::boolOpt(char c,bool* arg,const char* description) {
	return add(new BoolOpt(c,description,arg));
	}

class In32Opt:public  GetOpt::Option {
public:
  int32_t* ptr;
	In32Opt(char c,const char* desc,int32_t* ptr): GetOpt::Option(c,desc),ptr(ptr) {
		}
	virtual ~In32Opt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);*ptr=atoi(arg);}
};


GetOpt::Option& GetOpt::intOpt(char c,int32_t* arg,const char* description) {



	return add(new In32Opt(c,description,arg));
	}


class DoubleOpt:public  GetOpt::Option {
public:
  double* ptr;
	DoubleOpt(char c,const char* desc,double* ptr): GetOpt::Option(c,desc),ptr(ptr) {
		}
	virtual ~DoubleOpt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);*ptr=atof(arg);}
};


GetOpt::Option& GetOpt::doubleOpt(char c,double* arg,const char* description) {
	return add(new DoubleOpt(c,description,arg));
	}


class CStringOpt:public  GetOpt::Option {
public:
  char** ptr;
	CStringOpt(char c,const char* desc,char** ptr): GetOpt::Option(c,desc),ptr(ptr) {
		}
	virtual ~CStringOpt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);*ptr=const_cast<char*>(arg);}
};


GetOpt::Option& GetOpt::cStringOpt(char c,char** arg,const char* description) {
	return add(new CStringOpt(c,description,arg));
	}




class CCStringOpt:public  GetOpt::Option {
public:
  std::string* ptr;
	CCStringOpt(char c,const char* desc,std::string* ptr): GetOpt::Option(c,desc),ptr(ptr) {
		}
	virtual ~CCStringOpt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);ptr->assign(arg);}
};


GetOpt::Option& GetOpt::ccStringOpt(char c,std::string* arg,const char* description) {
	return add(new CCStringOpt(c,description,arg));
	}





class StringsOpt:public  GetOpt::Option {
public:
  std::vector<std::string>* ptr;
	StringsOpt(char c,const char* desc,std::vector<std::string>* ptr): GetOpt::Option(c,desc),ptr(ptr) {
		}
	virtual ~StringsOpt() {}
	virtual bool takes_arg() { return true;}
	virtual void process(const char* arg) {assert(arg!=NULL);ptr->push_back(arg);}
};


GetOpt::Option& GetOpt::stringsOpt(char c,std::vector<std::string>* arg,const char* description) {
	return add(new StringsOpt(c,description,arg));
	}



GetOpt::Option&  GetOpt::add(GetOpt::Option* opt) {
	for(size_t i=0;i< options.size();i++) {
      if(options.at(i)->optc==opt->optc) {
       fprintf (stderr, "duplicate option %c\n", opt->optc);
      	exit(EXIT_FAILURE);
      	}
       if(!opt->long_opt.empty() && options.at(i)->long_opt==opt->long_opt) {
       fprintf (stderr, "duplicate option %s\n", opt->long_opt.c_str());
      	exit(EXIT_FAILURE);
      	}
      }
  options.push_back(opt);
  return *opt;
	}


void GetOpt::usage(std::ostream& out) {
	for(size_t i=0;i< options.size();i++) {
      GetOpt::Option* opt= options.at(i);
      out << " -" << opt->optc << " " << opt->desc << std::endl;
      }
  out << std::endl;
	}

 int GetOpt::parse(int argc,char** argv) {
   std::string optstring;
   std::vector<struct option> loptions;
   
   for(size_t i=0;i< options.size();i++) {
      GetOpt::Option* opt= options.at(i);
			optstring+= opt->optc;
			if(opt->takes_arg()) optstring+=":";
			if(!opt->long_opt.empty()) {
				struct option lo={opt->long_opt.c_str(),opt->takes_arg()?required_argument:no_argument,NULL,opt->optc};
				loptions.push_back(	lo);
				}
		}
		
	 struct option eof={NULL,0,NULL,0};
	 loptions.push_back(	eof);
		
	 bool flag_help_seen=false;
   int opt;
   while ((opt = ::getopt_long(argc, argv, optstring.c_str(), loptions.data(),NULL)) >0) {
   			 if (opt == '?') {
	          fprintf (stderr, "Unknow option -%c %s\n", optopt, optstring.c_str());
	          return -1;
	          }
        else if (opt == ':') {
	          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
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
          fprintf (stderr, "Unknow option -%c.\n", opt);
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
			  fprintf (stderr, "Option %c was not set.\n", opt->optc);
			  return -1;
				}
			}
		while(optind <argc) {
			files.push_back(argv[optind++]);
			}	
     return 0;
     }
 