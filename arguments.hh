#ifndef ARGUMENTS_HH
#define ARGUMENTS_HH
#include <unistd.h>
#include <cstring>
#include <string>
#include <vector>
#include <cstdarg>
#include <getopt.h>
#include "debug.hh"

class Arguments {
	public:
		class Option {
			friend class ArgumentList;
			public:
				int shortOpt;
				std::string longOpt;
				std::string description;
				int has_arg;
				Arguments* owner;
			};
		std::vector<Arguments::Option*> options;
	
		struct Maker {
			Arguments::Option* option;
			
			Maker& hidden() {
				return *this;
				}
			Maker& longOpt(const char* longOpt) {
				if(longOpt!=NULL && std::strlen(longOpt)>0UL) {
					option->longOpt.assign(longOpt);
					}
				return *this;
				}
			Maker& description(const char* desc) {
				if(desc!=NULL && std::strlen(desc)>0UL) {
					option->description.assign(desc);
					}
				return *this;
				}
			};
	
		Arguments() {
			}
		virtual ~Arguments() {
			for(unsigned int i=0;i< options.size();i++) {
				delete options[i];
				}
			}
	protected:
		Maker make() {
				Maker m;
				m.option = new Option;
				m.option->owner = this;
				options.push_back(m.option);
				return m;
				}
	public:
		
		virtual bool handle(Option* opt) {
			return false;
			}
	};

class HelpArg:public Arguments {
	public:
		bool print_help_flag;
		bool print_version_flag;
		HelpArg():print_help_flag(false),print_version_flag(false) {
			make().longOpt("help").description("print help and exit");
			make().longOpt("version").description("print version and exit");
			}
		virtual bool handle(Arguments::Option* opt) {
			if(std::strcmp(opt->longOpt.c_str(),"help")==0) {
				print_help_flag = true;
				}
			if(std::strcmp(opt->longOpt.c_str(),"version")==0) {
				print_version_flag = true;
				}
			return true;
			}
	};

class ArgumentList {
	private:
		std::vector<Arguments*>  arguments;
		
		void fill(std::vector<Arguments::Option*>& v) {
			for(unsigned int i=0;i< arguments.size();i++) {
				for(unsigned int j=0;j< arguments[i]->options.size();j++) {
					v.push_back( arguments[i]->options[j]);
					}
				}
			}
	public:
		ArgumentList(Arguments* arg, ...) {
			// first
			if(arg!=NULL) arguments.push_back(arg);
		
			va_list va;
			/* initialize valist for num number of arguments */
		   va_start(va, arg);
		   /* access all the arguments assigned to valist */
		  for(;;)
		  		{
		  		Arguments* a = va_arg(va, Arguments*);
		  		if(a==NULL) break;
		     	arguments.push_back(a);
   				}
		   /* clean memory reserved for valist */
		   va_end(va);
		   }
	void usage(std::ostream& os) {
		std::vector<Arguments::Option*> options;
		fill(options);
		for(unsigned int i=0;i< options.size();i++) {
			os << "   ";
			if(options[i]->shortOpt > 0) {
				os << "-" << (char)options[i]->shortOpt;
				}
			os << " ";
			if(!options[i]->longOpt.empty()) {
				os << "--" << options[i]->longOpt;
				}
			if(!options[i]->description.empty()) {
				os << " " << options[i]->description;
				}
			os << std::endl;
			}
		}
	void parse(int argc, char** argv) {
		std::vector<Arguments::Option*> options;
		fill(options);
		std::string optstr;
		option* long_options = new option[options.size()+1];
		for(unsigned int i=0;i< options.size();i++) {
			long_options[i].name = options[i]->longOpt.c_str();
			if(options[i]->shortOpt>0) {
				optstr+=(char)options[i]->shortOpt;
				if(options[i]->has_arg) optstr+=":";
				}
			}
		//set zero to last item
		std::memset((void*)&long_options[options.size()],0,sizeof(option));
		int option_index = 0;
		for(;;) {
		 	int c = getopt_long(argc, argv,optstr.c_str(),long_options, &option_index);
		 	if(c==-1) break;
		 	if(c==0)
		 		{
		 		option* sel = &long_options[option_index];
		 		unsigned int i=0;
		 		for(i=0;i< options.size();i++) {
		 			if(std::strcmp(sel->name,options[i]->longOpt.c_str())==0) {
		 				break;
		 				}
		 			}
		 		if(i>= options.size()) THROW_ERROR("Cannot get long opt!");
		 		DEBUG("i="<< i);
		 		DEBUG("i="<< options[i]->owner);
		 		options[i]->owner->handle(options[i]);
		 		}
			}
		delete[] long_options;
		}
	};

#endif
