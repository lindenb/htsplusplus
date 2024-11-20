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

#ifndef BCF_RECORD_HH
#define BCF_RECORD_HH
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include "HtsFile.hh"
#include "Locatable.hh"
#include "debug.hh"

class BcfRecord {
	public:
		bcf1_t* bcf;
		BcfRecord() {
			bcf = ::bcf_init();
			if(bcf==NULL) {
			    	THROW_ERROR("Out of memory.");
    				}
			}
        BcfRecord(const BcfRecord& cp):bcf(::bcf_dup(cp.bcf)) {
            if(bcf==NULL) {
			    	THROW_ERROR("Out of memory.");
    				}
            }
	
		virtual ~BcfRecord() {
			::bcf_destroy(bcf);
			}

		
		bcf1_t* get() {return bcf;}
		bcf1_t* get() const {return bcf;}

		virtual int tid() {
			return get()->rid;
			}

		hts_pos_t length_on_reference() const {
			return get()->rlen;
			}
		virtual hts_pos_t start() const {
			return get()->pos;
			}
		virtual hts_pos_t end() const {
			return start() + length_on_reference() - 1 ;
			}
		virtual void unpack(int id) {
			::bcf_unpack(get(),id);
			}
		virtual void unpack_genotypes() {
			this->unpack(BCF_UN_IND);
			}
	};

class BcfRecordHeader :public BcfRecord, public Locatable {
	public:
		bcf_hdr_t *header;

		BcfRecordHeader(bcf_hdr_t *header):header(header) {
			}
		BcfRecordHeader():header(NULL) {
			}
        	BcfRecordHeader(const BcfRecordHeader& cp):BcfRecord(cp),header(cp.header) {
			}
		virtual ~BcfRecordHeader() {
			}
		
		virtual const char* contig() const {
			return bcf_seqname(header,get());
			}

  		virtual hts_pos_t start() const {
                        return BcfRecord::start();
                        }
                virtual hts_pos_t end() const {
                        return BcfRecord::end();
                        }

		void add_filter(int filter_id) {
			::bcf_add_filter(header,get(),filter_id);
		 	}
		 
		 void add_filter(const char* filter_id) {
			THROW_ERROR("TODO");
		 	}
		 
		virtual bcf_fmt_t* get_fmt(const char* format) {
			return ::bcf_get_fmt(this->header,get(),format);
			}
	};


#endif
