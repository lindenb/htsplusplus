#ifndef ABSTRACT_BAM_FILTER_H
#define ABSTRACT_BAM_FILTER_H
/*
The MIT License (MIT)

Copyright (c) 2022 Pierre Lindenbaum

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

#include "debug.hh"


class BamReader {
	public:
		virtual bool is_require_index() {
			return false;
			}
		virtual bool is_require_scan() {
			return true;
			}
		virtual int accept(hts_hdr* header,bam1_t* b) = 0;

		virtual void cleanup() {
			}

		int run(const char* filenamein) {
			int ret = EXIT_SUCCESS;
			htsFile* in = NULL;
			header = NULL;
			hts_idx_t *idx = NULL;
			bam1_t* b = NULL;
			in = hts_open(fnamein,"r");
			if(this->in==NULL) {
    				ERROR("Cannot open input %s. (%s)",(optind==argc ?"<stdin>":argv[optind]),strerror(errno));
				ret = EXIT_FAILURE;
				goto bail;
    				}
			header = validateHeader(sam_hdr_read(in));
			if(header==NULL) {
				hts_close(this->in);
			 	ERROR("Cannot read header for input %s.",argv[optind]);
				ret = EXIT_FAILURE;
				goto bail;
    				}
			if(is_require_index()) {
				this->idx = sam_index_load(in,filenamein);
				if(idx==NULL) {
					ERROR("Cannot read index for %s.\n",argv[optind]);
					}
				}
			if(is_require_scan()) {
				b = bam_init1();
				if(b==NULL) {
					ERROR("Out of memory.");
					ret= EXIT_FAILURE;
					goto bail;
    				    	}
			if((ret=beforeScan(header))!=EXIT_SUCCESS) {
				goto bail;
				}
			while((ret= sam_read1(in,header, b))>=0) {
				int ret2 = accept(header,b);
				switch(ret2) {
					case SUCCESS:break;
					case ERROR: ret = EXIT_FAILURE; goto bail; break;
					case STOP: goto bail; break;
					default: FATAL("ILLEGAL STATE");break;
					}
				}
				}
			ret = afterScan();
			bail:
				cleanup();
				if(b!=NULL) bam_destroy1(b);
				if(idx!=NULL) hts_idx_destroy(idx);
				if(header!=NULL) bam_hdr_destroy(header);
				if(in!=NULL) hts_close(in);
			return ret;
			}
		};


class BamFilter : public BamReader{
	private:
		std::filename_out;
		fpw;
		hdr;
	public:
		int beforeScan(header* h) {
		    if (sam_hdr_write(fpw, h) < 0) {
		        print_error_errno("collate", "Couldn't write header");
		        goto fail;
			}
		   }

		int write(hts_hdr* header,bam1_t* b) {
                    if (sam_write1(this->fpw, h, r1) < 0) {
                        ERROT("cannot write alignment");
			return EXIT_FAILURE;
			}
		  return EXIT_SUCCESS;
	          }
	
	virtual int accept(hts_hdr* header,bam1_t* b) {
                    }
	
	int run(const char* filenamein,const char* filenameout) {
		this->filename_out.assign(filenameout);
		int ret= run(filenamein);
		if(
		return ret;
		}
	virtual void cleanup() {
		}
	};
