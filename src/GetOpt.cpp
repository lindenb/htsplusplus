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

GetOpt::GetOpt() {
}

GetOpt::~GetOpt() {
}

void  GetOpt::add(shared_ptr<Option> opt) {
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
      }
  options.push_back(opt);
	}


void GetOpt::usage(std::ostream& out) {
	for(size_t i=0;i< options.size();i++) {
      shared_ptr<Option> opt= options.at(i);
      if(opt->flag_hidden) continue;
      out << " ";
      if(opt->optc!=0) {
      	out << ANSI_GREEN( "-" << (char)opt->optc);
      	if(!opt->long_opt.empty()) out << "|";
      	}
      if(!opt->long_opt.empty()) {
      	out << ANSI_GREEN( "--" << opt->long_opt);
      	}
      if(!opt->is_flag()) out << ANSI_BLUE( " <" << opt->arg_name << ">" ) ;  
      out <<  " " << ansi::white << opt->description << ansi::reset;
      if(opt->flag_required)   out <<  " " << ANSI_YELLOW( "[REQUIRED]");
     	if(!opt->is_flag()) {
			out<< ". " << ansi::cyan << "[" << opt->description << "]" << ansi::reset ;
			}
      out << std::endl;
      }
  out << std::endl;
	}

 int GetOpt::parse(int argc,char** argv) {
   std::string optstring;
   std::vector<struct option> loptions;
   for(size_t i=0;i< options.size();i++) {
      std::shared_ptr<Option> opt= options.at(i);
      if(opt->optc==0 && opt->long_opt.empty()) {
      	THROW_ERROR("option.c=0 && option.longopt.empty()");
      }
      
      if(opt->optc!=0) {
				optstring+= opt->optc;
				if(!opt->is_flag()) optstring+=":";
				}
			
			if(!opt->long_opt.empty()) {
				struct option lo={opt->long_opt.c_str(),!opt->is_flag()?required_argument:no_argument,NULL,opt->optc};
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
       Option* option= NULL;
       for(size_t i=0;i< options.size();i++) {
            if(options.at(i)->optc==opt) {
            	option = options.at(i).get();
            	break;
            	}
          }

       if(option==NULL) {
 	      	cerr << ansi::red << "Unknow option `-" << (char)opt << "` \n" << ansi::reset;
       		return -1;
       		}
       option->count_seen++;
       if(option->process(!option->is_flag()?optarg:NULL)!=0) {
		return -1;
		}
       }

      for(size_t i=0;i< options.size() && !flag_help_seen ;i++) {
      shared_ptr<Option> opt= options.at(i);
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

