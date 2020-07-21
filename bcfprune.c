/*
The MIT License (MIT)

Copyright (c) 2020 Pierre Lindenbaum

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
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <htslib/hts.h>
#include <htslib/vcf.h>

#define WHERE do {fprintf(stderr,"[%s:%d]",__FILE__,__LINE__);} while(0)
#define WARNING(...) do { fputs("[WARNING]",stderr);WHERE;fprintf(stderr,__VA_ARGS__);fputc('\n',stderr);} while(0)
#define ERROR(...) do { fputs("[ERROR]",stderr);WHERE;fprintf(stderr,__VA_ARGS__);fputc('\n',stderr);abort();} while(0)

static void usage(const char* name,FILE* out) {
    fprintf(out,"%s: Compiled %s %s. Pierre Lindenbaum\n",name,__DATE__,__TIME__);
    fprintf(out,"Usage: %s [ -O (o|v|z) ] [-o fileout] -d <distance> (stdin|bcf)\n",name);
    fprintf(out,"Options:\n");
    fprintf(out,"  -h print help\n");
    fprintf(out,"  -d (int) distance\n");
    fprintf(out,"  -o (file) output file (default stdout)\n");
    fprintf(out,"  -O (char) output format z:gzip vcf v:vcf b:bcf (default v)\n");
    fprintf(out,"\n");
    }

int main(int argc,char** argv) {
int distance=-1;
char* fileout=NULL;
int c;
char format[3]={'v','w',0};
 while ((c = getopt (argc, argv, "o:O:d:h")) != -1)
    switch (c)
      {
      case 'O':
            if(strlen(optarg)!=1) {
                usage(argv[0],stderr);
                ERROR("Bad format '%s'.",optarg);
                return EXIT_FAILURE;                
                }
            format[0]=optarg[0];
            break;
      case 'o': fileout=strdup(optarg);break;
      case 'h':
            usage(argv[0],stdout);
            return EXIT_SUCCESS;
      case 'd':
            distance = atoi(optarg);
            break;
      default:
          usage(argv[0],stderr);
          ERROR("Argument error.");
          return EXIT_FAILURE;
      }
if(distance<=0)  {
    usage(argv[0],stderr);    
    ERROR("Bad distance '%d' <=0.",distance);
    return EXIT_FAILURE;    
    }
if(!(optind==argc || optind<argc)) {
    ERROR("Illegal number of arguments");
    return EXIT_FAILURE;    
    }
htsFile *in = hts_open(optind==argc ?"-":argv[optind],"r");
if(in==NULL) {
    ERROR("Cannot open input vcf %s. (%s)",(optind==argc ?"<stdin>":argv[optind]),strerror(errno));
    return EXIT_FAILURE;    
    }
bcf_hdr_t *header = bcf_hdr_read(in);
if(header==NULL) {
    ERROR("Cannot read header for input vcf %s.",(optind==argc ?"<stdin>":argv[optind]));
    return EXIT_FAILURE;    
    }
htsFile *out = hts_open((fileout==NULL?"-":fileout),format);
if(out==NULL) {
    ERROR("Cannot open output vcf %s. (%s)",(fileout==NULL?"<STDOUT>":fileout),strerror(errno));
    return EXIT_FAILURE;    
    }

if ( bcf_hdr_write(out, header)!=0 ) {
    ERROR("Cannot write header.");
    return EXIT_FAILURE;      
    }

bcf1_t* bcf = bcf_init();
if(bcf==NULL) {
    ERROR("Out of memory.");
    return EXIT_FAILURE;    
    }
hts_pos_t prev_end = -1;
hts_pos_t prev_start = -1;
int32_t prev_tid=-1;
int ret=0;
while((ret=bcf_read(in, header, bcf))==0) {
       if(bcf->errcode!=0) {
            WARNING("Skipping Error in VCF record at tid %d:%"PRIhts_pos,bcf->rid, bcf->pos+1);
            continue;
            }
       if(prev_tid>=0 && (prev_tid> bcf->rid || (prev_tid==bcf->rid && bcf->pos < prev_start))) {
            ERROR("Data are not ordered (tid %d:%"PRIhts_pos") vs (tid %d:%"PRIhts_pos").",prev_tid,prev_start+1,bcf->rid,bcf->pos+1);
            return EXIT_FAILURE;        
            }
       if(bcf->rid!=prev_tid || (bcf->pos - prev_end) >= distance ) {
           if(bcf_write1(out, header, bcf)!=0) {
                ERROR("IO error. Cannot write record.");
                return EXIT_FAILURE;
                }
            prev_tid = bcf->rid;
            prev_start = bcf->pos;
            prev_end = bcf->pos+bcf->rlen-1;
            }
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
