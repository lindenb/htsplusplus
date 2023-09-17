#ifndef BCF_HEADER_HH
#define BCF_HEADER_HH
#include <htslib/hts.h>
#include <htslib/vcf.h>
#include "HtsFile.hh"
#include "debug.hh"

class BcfHeader {
	public:
		bcf_hdr_t *header;

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
		virtual int nsamples() {
			return bcf_hdr_nsamples(get());
			}
		virtual bool has_samples() {
			return nsamples() > 0;
			}
		 virtual int sample_index(const char* sn) {
		 	return ::bcf_hdr_id2int(get(),BCF_DT_SAMPLE,sn);
			}
		virtual const char* sample(int idx) {
			return get()->samples[idx];
			}
		
		static std::unique_ptr<BcfHeader> read(htsFile *fp);
	};

#endif

