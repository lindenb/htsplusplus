#ifndef HTSFILE_HPP
#define HTSFILE_HPP

#include <cerrno>
#include <stdexcept>
#include <sstream>
#include <htslib/hts.h>

class HtsFile {
	public:
		htsFile *fp;
		HtsFile():fp(NULL) {
			}
		virtual bool is_open() {
			return fp!=NULL;
			}
		virtual void close() {
			if(fp!=NULL) {
				::hts_close(fp);
				fp = NULL;
				}
			}
		virtual ~HtsFile() {
			close();
			}
		virtual bool  open(const char* filename,const char* mode) {
			close();
			errno = 0;
			fp = hts_open(filename,mode);
			return is_open();
			}
		virtual void openOrFail(const char* filename,const char* mode) {
			if(!open(filename,mode)) {
				std::stringstream os;
				os << "Cannot open \"" << filename << "\" mode:\"" << mode << "\":" << strerror(errno);
				throw std::runtime_error(os.str());
				}
			}
	};

#endif

