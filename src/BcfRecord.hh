#ifndef BCF_RECORD_HH
#define BCF_RECORD_HH
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include "HtsFile.hh"
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
		
		bcf1_t* get() {
			return bcf;
			}
	
		virtual ~BcfRecord() {
			::bcf_destroy(bcf);
			}
		int tid() {
			return get()->rid;
			}

		hts_pos_t length_on_reference() {
			return get()->rlen;
			}
		hts_pos_t start() {
			return get()->pos;
			}
		hts_pos_t end() {
			return start() + length_on_reference() - 1 ;
			}
	};

class BcfRecordHeader :public BcfRecord {
	public:
		bcf_hdr_t *header;

		BcfRecordHeader(bcf_hdr_t *header):header(header) {
			}
		BcfRecordHeader():header(NULL) {
			}
		virtual ~BcfRecordHeader() {
			}
	};


#endif