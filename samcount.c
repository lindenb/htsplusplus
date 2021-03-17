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
#include <vector>
#include <unistd.h>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include <htslib/vcf.h>
#include <htslib/kseq.h>


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

#define BAM_MAX_BIN 37450 // =(8^6-1)/7+1
static int reg2bins(uint32_t beg, uint32_t end, uint16_t list[BAM_MAX_BIN])
{
	int i = 0, k;
	if (beg >= end) return 0;
	if (end >= 1u<<29) end = 1u<<29;
	--end;
	list[i++] = 0;
	for (k =    1 + (beg>>26); k <=    1 + (end>>26); ++k) list[i++] = k;
	for (k =    9 + (beg>>23); k <=    9 + (end>>23); ++k) list[i++] = k;
	for (k =   73 + (beg>>20); k <=   73 + (end>>20); ++k) list[i++] = k;
	for (k =  585 + (beg>>17); k <=  585 + (end>>17); ++k) list[i++] = k;
	for (k = 4681 + (beg>>14); k <= 4681 + (end>>14); ++k) list[i++] = k;
	return i;
}

static void read_bed(const char* fname,sam_hdr_t *h,vector<TidStartEnd*>& beds) {
    int ret = 0;
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
		
		int tid = bam_name2id(h, (char*)str.s);
		if(tid<0) {
			fprintf(stderr,"unknown chromosome %s in %s\n",str.s,fname);
			exit(EXIT_FAILURE);
			}
		TidStartEnd* rec= new TidStartEnd;
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
vector<TidStartEnd*> beds;
vector<vector<TidStartEnd*>*> tid2bin;
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
if(optind==argc) {
    ERROR("Illegal number of arguments");
    return EXIT_FAILURE;    
    }
htsFile *in = hts_open(argv[optind],"r");
if(in==NULL) {
    ERROR("Cannot open input %s. (%s)",(optind==argc ?"<stdin>":argv[optind]),strerror(errno));
    return EXIT_FAILURE;    
    }
sam_hdr_t *header = sam_hdr_read(in);
if(header==NULL) {
    ERROR("Cannot read header for input %s.",argv[optind]);
    return EXIT_FAILURE;    
    }
hts_idx_t *idx = sam_index_load(in,argv[optind]);
	if(idx==NULL) {
	fprintf(stderr,"Cannot read index for %s.\n",argv[optind]);
	exit(EXIT_FAILURE);
	}

read_bed(bedfname,header,beds);
if(beds.empty()) {
	ERROR("no bed record in %s.",bedfname);
	return EXIT_FAILURE;    
	}

for(unsigned int i=0;i< beds.size();i++) {
	TidStartEnd* rec = beds[i];

	while(rec->tid >= (int32_t)tid2bin.size()) {
		tid2bin.push_back(new vector<TidStartEnd*>);
		}

	vector<TidStartEnd*>*   binmap = tid2bin[rec->tid];

	int32_t binid = hts_reg2bin(rec->start,rec->end,14,5);
	while(binid>= (int32_t)binmap.size()) {
		binmap.push_back(new vector<TidStartEnd*>);
		}
	binmap->push_back(rec);
	}
    
FILE *out = fileout==NULL?stdout:fopen(fileout),"w");
if(out==NULL) {
    ERROR("Cannot open output %s. (%s)",(fileout==NULL?"<STDOUT>":fileout),strerror(errno));
    return EXIT_FAILURE;
    }

bam1_t* b = bam_init1();
if(b==NULL) {
    ERROR("Out of memory.");
    return EXIT_FAILURE;    
    }

int32_t prev_tid=-1;
int ret=0;

uint16_t*	bins = (uint16_t*)calloc(BAM_MAX_BIN, 2);

while((ret= sam_read1(in,header, b))>=0) {
      if( b->core.flag & (BAM_FUNMAP | BAM_FSECONDARY | BAM_FQCFAIL | BAM_FDUP) ) continue;      
      int tid =  b->core.tid;
      if(tid<0 || tid >=  tid2bin.size()) continue;
      vector<vector<TidStartEnd*>*> binmap = tid2bin[tid];
      if(binmap.empty()) continue;
      int beg = b->core.pos
      int end = bam_endpos(b);
      int n_bins = reg2bins(beg, end, bins);
      for(int i=0;i< nÃ_bins;i++) {
	int bin = bins[i];
	if(bin>=binmap->size()) continue;
	for(size_t j=0;j<
	}

      prev_tid=0;
      }
hts_idx_destroy(idx);
bam_destroy1(b);
bam_hdr_destroy(header);
hts_close(in);
fclose(out);
free(fileout);
if(ret<-1) {
    ERROR("IO error in input VCF.\n");
    return EXIT_FAILURE;
    }
return EXIT_SUCCESS;
}
