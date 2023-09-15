#include <memory>
#include "SamFileHeader.hh"
#include "debug.hh"


std::unique_ptr<SamFileHeader> SamFileHeader::read(samFile *fp) {
			sam_hdr_t *header = ::sam_hdr_read(fp);
			if(header==NULL) {
			    THROW_ERROR("Cannot read header for input bam.");
    				}
			return std::unique_ptr<SamFileHeader>(new SamFileHeader(header));
			}

