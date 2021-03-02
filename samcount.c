/*
The MIT License (MIT)

Copyright (c) 2021 Pierre Lindenbaum

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
#include <cerrno>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <unistd.h>
#include <htslib/hts.h>
#include <htslib/vcf.h>

using namespace std;
#define WHERE do {fprintf(stderr,"[%s:%d]",__FILE__,__LINE__);} while(0)
#define WARNING(...) do { fputs("[WARNING]",stderr);WHERE;fprintf(stderr,__VA_ARGS__);fputc('\n',stderr);} while(0)
#define ERROR(...) do { fputs("[ERROR]",stderr);WHERE;fprintf(stderr,__VA_ARGS__);fputc('\n',stderr);abort();} while(0)

struct TidStartEnd {
	int32_t tid;
	int32_t start;
	int32_t end;
	long count;
	};

static void read_bed(const char* fname,sam_hdr_t *h,vector<TidStartEnd*>& beds) {
    kstring_t str = KS_INITIALIZE;
    htsFile *fp= hts_open(fname,"r");
    if ( fp == NULL ) {
    	fprintf(stderr,"Cannot open %s. %s\n",fname,strerror(errno));
    	exit(EXIT_FAILURE);
    }

    while ((ret = hts_getline(fp, KS_SEP_LINE, &str)) > 0 ) {
        char *ss = (char*) str.s;
        if ( ss[0]=='#' ) continue;
		while ( *ss && !*ss=='\t' ) ss++;
		if (*ss == 0 )  continue;
		
		char *se = ss;
		while ( *se && !*se=='\t' ) se++;
		

		*ss = 0;
		*se = 0;
		
		int tid = sam_hdr_name2tid(h, str.s);
		if(tid<0) {
			fprintf(stderr,"unknown chromosome %s in %s\n",str.s,fname);
			exit(EXIT_FAILURE);
			}
		TidStartEnd rec= new TidStartEnd;
		rec->tid=tid;
		rec->start = atoi(ss);
		rec->end = atoi(ss);
		rec->count = 0L;
		beds.push_back(rec);
		
   		}
   
   hts_close(fp);
   ks_free(&str);
    }


static void usage(const char* name,FILE* out) {
    fprintf(out,"%s: Compiled %s %s. Pierre Lindenbaum\n",name,__DATE__,__TIME__);
    fprintf(out,"Usage: %s [ -O (o|v|z) ] [-o fileout] -d <distance> (stdin|bcf)\n",name);
    fprintf(out,"Options:\n");
    fprintf(out,"  -h print help\n");
    fprintf(out,"  -B (file) bed file\n");
    fprintf(out,"  -o (file) output file (default stdout)\n");
    fprintf(out,"\n");
    }

int samcount_main(int argc,char** argv) {
	vector<TidStartEnd> beds;
	map<int,map<int,vector<TidStartEnd*>* > > tid2bin;
char* bedfname = NULL;
char* fileout=NULL;
int c;
 while ((c = getopt (argc, argv, "o:B:h")) != -1)
    switch (c)
      {
      case 'o': fileout=strdup(optarg);break;
      case 'h':
            usage(argv[0],stdout);
            return EXIT_SUCCESS;
      case 'B':
            bedfname = optarg;
            break;
      default:
          usage(argv[0],stderr);
          ERROR("Argument error.");
          return EXIT_FAILURE;
      }
if(bedfname==NULL) {
	ERROR("undefined bed file");
    return EXIT_FAILURE;  
	}
if(!(optind==argc) {
    ERROR("Illegal number of arguments");
    return EXIT_FAILURE;    
    }
htsFile *in = hts_open(argv[optind],"r");
if(in==NULL) {
    ERROR("Cannot open input vcf %s. (%s)",(optind==argc ?"<stdin>":argv[optind]),strerror(errno));
    return EXIT_FAILURE;    
    }
bcf_hdr_t *header = bcf_hdr_read(in);
if(header==NULL) {
    ERROR("Cannot read header for input vcf %s.",(optind==argc ?"<stdin>":argv[optind]));
    return EXIT_FAILURE;    
    }
    
read_bed(bedfname,header,beds);
if(beds.empty()) {
	ERROR("no bed record in %s.",bedfname);
    return EXIT_FAILURE;    
	}
for(unsigned int i=0;i< beds.size();i++) {
	map<int,vector<TidStartEnd*>* > binmap = NULL;
	if((r=tid2bin.find(beds[i].tid))==tid2bin.end()) {
		binmap = new map<int,vector<TidStartEnd*> >;
		tid2bin.insert(make_pair(beds[i].tid,binmap));
		}
	else{
		binmap = r.second;
		}
	int binid = binOf(beds[i].start,beds[i].end);
	vector<TidStartEnd*>* v = NULL;
	if((r2=binmap->find(binid))==tid2bin.end()) {
		v = new vector<TidStartEnd*> ;
		tid2bin.insert(make_pairbinid,v));
		}
	else{
		v = r2.second;
		}
	v->push_back(
	}
    
FILE *out = fopen((fileout==NULL?"-":fileout),"w");
if(out==NULL) {
    ERROR("Cannot open output %s. (%s)",(fileout==NULL?"<STDOUT>":fileout),strerror(errno));
    return EXIT_FAILURE;    
    }

bcf1_t* bcf = bcf_init();
if(bcf==NULL) {
    ERROR("Out of memory.");
    return EXIT_FAILURE;    
    }

int32_t prev_tid=-1;
int ret=0;
while((ret=bcf_read(in, header, bcf))==0) {
      
      }
if(fileout) free(fileout);
bcf_destroy(bcf);
bcf_hdr_destroy(header);
hts_close(in);
hts_close(out);
if(ret<-1) {
    ERROR("IO error in input VCF.\n");
    return EXIT_FAILURE;
    }
return EXIT_SUCCESS;
}
