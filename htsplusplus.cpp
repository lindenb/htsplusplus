#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "arguments.hh"

using namespace std;

extern int bcfprune_main(int,char**);


int main(int argc,char** argv) {
	HelpArg arg1;
	ArgumentList app(&arg1, NULL);
	app.parse(argc, argv);
	if(arg1.print_help_flag) {
		cerr << "Options:" << endl;
		app.usage(cout);
		return 0;
		}

	if(argc<2) {
		fprintf(stderr,"%s. Sub-commands.\n",argv[0]);
		fprintf(stderr," bcfprune\n");
		fprintf(stderr," samcount\n");
		fprintf(stderr," bcf-exclude-bed\n");
		return EXIT_FAILURE;
		}
	if(strcmp(argv[1],"bcfprune")==0) {
		return bcfprune_main(argc-1,&argv[1]);
		}
	
	fprintf(stderr,"Unknown sub command \"%s\".\n",argv[1]);
	return EXIT_FAILURE;
	}
