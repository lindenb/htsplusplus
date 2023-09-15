#ifndef SAM_FILE_HEADER_HH
#define SAM_FILE_HEADER_HH
#include <htslib/hts.h>
#include <htslib/sam.h>
#include "HtsFile.hh"
#include "debug.hh"

class SamFileHeader {
	public:
		sam_hdr_t* header;

		SamFileHeader( sam_hdr_t *header):header(header) {
			}

		virtual ~SamFileHeader() {
			if(header!=NULL) ::sam_hdr_destroy(header);
			}
		void write(htsFile* fp) {
			if ( fp==NULL || header==NULL || ::sam_hdr_write(fp, header)!=0 ) {
				THROW_ERROR("Cannot write SAM header");
				}
			}
		sam_hdr_t* get() {
			return header;
			}
		sam_hdr_t* operator()() {
			return get();
			}

		static std::unique_ptr<SamFileHeader> read(samFile *fp);
	};

#endif

