#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include "Program.hh"
#include "SelfTest.hh"
#include "Rimbert20241119.hh"

using namespace std;
using namespace htspp;
class HtsPlusPlus: public Program {
public:
    std::vector<Program*> subprograms;
    HtsPlusPlus():Program("hts++","hts++") {
        subprograms.push_back(new SelfTest);
        subprograms.push_back(new Rimbert20241119);
    }
    
    virtual ~HtsPlusPlus() {
    }
    virtual void list(std::ostream& out) {
        out << "available programs:" << endl;
        for(auto i=subprograms.begin(); i!=subprograms.end();++i) {
            if((*i)->is_hidden()) continue;
            out << " - " << (*i)->name() << "\t" << (*i)->description() << endl;
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
        return subprog->main(argc - optind, &argv[optind]);
        }
};



int main(int argc,char** argv) {
    
	try {
	    HtsPlusPlus app;
		return app.main(argc, argv);
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
