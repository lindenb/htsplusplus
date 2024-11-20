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
