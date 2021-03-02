#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int bcfprune_main(int,char**);
extern int samcount_main(int,char**);

int main(int argc,char** argv) {
	if(argc<2) {
		fprintf(stderr,"%s. Sub-commands.\n",argv[0]);
		fprintf(stderr," bcfprune\n");
		fprintf(stderr," samcount\n");
		return EXIT_FAILURE;
		}
	if(strcmp(argv[1],"bcfprune")==0) {
		return bcfprune_main(argc-1,&argv[1]);
		}
	if(strcmp(argv[1],"samcount")==0) {
		return samcount_main(argc-1,&argv[1]);
		}
	fprintf(stderr,"Unknown sub command \"%s\".\n",argv[1]);
	return EXIT_FAILURE;
	}
