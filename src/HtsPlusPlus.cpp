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

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include <iomanip>
#include "ANSI.hh"
#include "Program.hh"
#include "SelfTest.hh"
#include "ProgramOnePassBam.hh"
//#include "ShiftSam.hh"
//#include "Rimbert20241119.hh"

using namespace std;
using namespace htspp;
class HtsPlusPlus: public Program {
public:
    std::vector<Program*> subprograms;
    HtsPlusPlus():Program("hts++","hts++") {
        subprograms.push_back(new ProgramOnePassBam("x","y"));
        //subprograms.push_back(new SelfTest);
        //subprograms.push_back(new ShiftSam);
    }
    
    virtual ~HtsPlusPlus() {
    }
    virtual void list(std::ostream& out) {
        out << "available programs:" << endl;
        for(auto i=subprograms.begin(); i!=subprograms.end();++i) {
            if((*i)->is_hidden()) continue;
            out << ANSI_GREEN( " - " << setw(10) << (*i)->name()) << "\t" << ANSI_BLUE( (*i)->description() ) << endl;
            }
        out << endl;
        }
    virtual int main(int argc,char** argv) {
	int optind=1;
	while(optind<argc) {
		if(strcmp(argv[optind],"-h")==0 || strcmp(argv[optind],"--help")==0) {
			list(cout);
			return 0;
			}
		else if(strcmp(argv[optind],"-v")==0 || strcmp(argv[optind],"--version")==0) {
			cout << "VERSION" << endl;
			return 0;
			}
		else if(strcmp(argv[optind],"--")==0) {
		    optind++;
		    break;
		    }
		else
			{
			break;
			}
		++optind;
		}
	if(optind==argc) {
		 list(cerr);
		 return -1;
		}

        char* subcmd = argv[optind];
        Program* subprog = NULL;
         for(auto i=subprograms.begin(); i!=subprograms.end();++i) {
            if(strcmp((*i)->name(), subcmd)==0) {
                subprog = *(i);
                break;
                }
            }

        if(subprog==NULL) {
            cerr << "unknown program \"" << subcmd << "\"" << endl;
	    	list(cerr);
            return -1;
            }
        int ret= subprog->main(argc - optind, &argv[optind]);
		if(ret!=0) {
			std::cerr << "Program exited with error" << std::endl;
			}
		return ret;
        }
};



int main(int argc,char** argv) {
    
	try {
	    HtsPlusPlus app;
		int ret= app.main(argc, argv);
		if(ret!=0) {
			std::cerr << "Program exited with error" << std::endl;
			}
		}
	catch(exception& err) {
		std::cerr << "Error: " << err.what() << std::endl;
		return EXIT_FAILURE;
		}
	catch(...) {
		std::cerr << "Error: Exiting." << std::endl;
		return EXIT_FAILURE;
		}
	}
