#include "BcfHeader.hh"
#include "debug.hh"


std::unique_ptr<BcfHeader> BcfHeader::read(htsFile *fp) {
			bcf_hdr_t *header = ::bcf_hdr_read(fp);
			if(header==NULL) {
			    THROW_ERROR("Cannot read header for input vcf.");
    				}
			return std::unique_ptr<BcfHeader>(new BcfHeader(header));
			}

