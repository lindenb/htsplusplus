#ifndef BCF_HEADER_HH
#define BCF_HEADER_HH
#include <htslib/hts.h>
#include <memory>
#include <htslib/vcf.h>
#include "HtsFile.hh"
#include "debug.hh"

class BcfHeader {
	public:
		typedef int filter_type;
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
		 	ASSERT_NOT_NULL(sn);
		 	return ::bcf_hdr_id2int(get(),BCF_DT_SAMPLE,sn);
			}
		virtual const char* sample(int idx) {
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
		
		
		static std::unique_ptr<BcfHeader> read(htsFile *fp);
	};

#endif

