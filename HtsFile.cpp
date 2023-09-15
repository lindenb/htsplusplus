#include "debug.hh"
#include "HtsFile.hh"

std::unique_ptr<HtsFile> HtsFile::open(const char* filename,const char* mode) {
	htsFile* fp;
          errno = 0;
          if((fp = ::hts_open((filename==NULL?"-":filename),mode))==NULL) {
		THROW_ERROR("Cannot open \"" << (filename==NULL?"NULL":filename) << "\" mode:\"" << mode << "\":" << strerror(errno));
		}
	return std::unique_ptr<HtsFile>(new HtsFile(fp));
	}
