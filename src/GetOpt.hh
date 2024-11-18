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
	  std::string desc;
	  std::string long_opt;
	  bool flag_required;
	  bool flag_help;
	  bool flag_hidden;
	  int count_seen;
	  char optc;
	  Option(char c,const char* desc);
	  virtual ~Option();
		Option& required();
		Option& help();
		Option& hidden();
		Option& longOpt(const char*);
		virtual void touch();
		virtual void process(const char* arg_or_NULL)=0;
		virtual bool takes_arg()=0;
	};

private:
std::vector<GetOpt::Option*> options;
GetOpt::Option& add(GetOpt::Option* opt);
public:
  std::vector<char*> files;

	GetOpt();
	~GetOpt();
	void usage(std::ostream& out);
	Option& boolOpt(char c,bool* arg,const char* description);
 	Option& intOpt(char c,int32_t* arg,const char* description);
  Option& doubleOpt(char c,double* arg,const char* description);
 	Option& cStringOpt(char c,char** arg,const char* description);
 	Option& ccStringOpt(char c,std::string* arg,const char* description);
 	Option& stringsOpt(char c,std::vector<std::string>* arg,const char* description);
 	int parse(int argc,char** argv);
};


}





#endif
