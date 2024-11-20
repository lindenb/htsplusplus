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

#ifndef BCF_HEADER_HH
#define BCF_HEADER_HH
#include <iostream>
#include <vector>
#include <memory>
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include "SamSequenceDictionary.hh"
#include "HtsFile.hh"
#include "utils.hh"
#include "debug.hh"

class BcfHeader {
	public:
		typedef int filter_type;
		bcf_hdr_t *header;

	public:
		BcfHeader( bcf_hdr_t *header):header(header) {
			}
		BcfHeader(const BcfHeader& cp):header(cp.header==NULL?NULL: ::bcf_hdr_dup(cp.header)) {
			if (cp.header!=NULL && header==NULL) {
				THROW_ERROR("Cannot copy VCF header");
				}
			}

		virtual ~BcfHeader() {
			if(header!=NULL) ::bcf_hdr_destroy(header);
			}


		void write(htsFile* fp) {
			if ( fp==NULL || header==NULL || ::bcf_hdr_write(fp, header)!=0 ) {
				THROW_ERROR("Cannot write VCF header");
				}
			}
		bcf_hdr_t* get() {
			return header;
			}
		bcf_hdr_t* operator()() {
			return get();
			}
		std::unique_ptr<BcfHeader> clone() {
			return std::unique_ptr<BcfHeader>(new BcfHeader(*this));
			}
		virtual unsigned int nsamples() {
			return bcf_hdr_nsamples(get());
			}
		virtual bool has_samples() {
			return nsamples() > 0;
			}
        virtual std::vector<std::string> samples() {
            std::vector<std::string> L;
            L.reserve(nsamples());       
            for(unsigned i=0;i< nsamples(); i++) {
                L.push_back( sample(i) );           
                }
            return L;
            }

		 virtual int sample_index(const char* sn) {
		 	ASSERT_NOT_NULL(sn);
		 	return ::bcf_hdr_id2int(get(),BCF_DT_SAMPLE,sn);
			}
		virtual const char* sample(unsigned int idx) {
            if(idx>=nsamples()) THROW_ERROR("index out of bounds");
			return get()->samples[idx];
			}
		virtual void append(const char* s) {
			ASSERT_NOT_NULL(s);
			if(::bcf_hdr_append(get(),s)!=0) {
				THROW_ERROR("cannot insert line \"" << s << "\".");
				}
			}
			
		virtual void printf(const char *fmt, ...) {
			va_list ap;
			//first eval length
			va_start(ap, fmt);
			int len = vsnprintf(nullptr,0, fmt, ap);
			va_end(ap);
	
			if (len > 0) {
				char* line = (char*)::malloc(len+1);
				ASSERT_NOT_NULL(line);

				va_start(ap, fmt);
				vsnprintf(line, len, fmt, ap);
				va_end(ap);
				this->append(line);
				::free(line);
				}
			}
		

		
		virtual BcfHeader::filter_type add_filter(const char* filter_id,const char* description) {
			ASSERT_NOT_NULL(filter_id);
			ASSERT_NOT_NULL(description);
			if( bcf_hdr_id2int(get(), BCF_DT_ID, filter_id) >=0 ) {
				THROW_ERROR("##FILTER=" << filter_id << " already defined in header.");
				}
			this->printf("##FILTER=<ID=%s,Description=\"%s\">", filter_id,description);
    		filter_type filter_id2 = bcf_hdr_id2int(get(), BCF_DT_ID, filter_id);
  			if(filter_id2 < 0) {
  				THROW_ERROR("cannot add filter " << filter_id);
  				}
  			return filter_id2;
			}
		
		
		
		virtual BcfHeader::filter_type add_filter(const char* filter_id) {
			return add_filter(filter_id,filter_id);
			}
		
		virtual std::ostream& write(std::ostream& out) {
    			kstring_t htxt = {0,0,0};
			    if (bcf_hdr_format(get(), 0, &htxt) < 0) {
			        free(htxt.s);
				THROW_ERROR("Cannot write header");
    				}
			while (htxt.l && htxt.s[htxt.l-1] == '\0') --htxt.l; // kill trailing zeros
			out.write(htxt.s, htxt.l);
			free(htxt.s);
			return out;
			}


		static std::unique_ptr<BcfHeader> read(htsFile *fp);
	};

#endif

