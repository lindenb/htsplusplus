#include "debug.hh"
#include "HtsFile.hh"

using namespace std;
using namespace htspp;

HtsFile::HtsFile(htsFile *fp,bool b):Pointer(fp),close_in_descructor(b) {
    }

int HtsFile::set_opt(hts_fmt_option opcode,int value) {
	 return ::hts_set_opt(get(), opcode, value);
	}

void HtsFile::close() {
		if(!is_null()) {
			if(close_in_descructor)	::hts_close(get());
			this->ptr = NULL;
			}
		}
bool HtsFile::is_open() {
	return is_not_null();
	}

HtsFile::~HtsFile() {
	close();
	}

void HtsFile::set_reference(const char *fn_aux) {
		ASSERT_NOT_NULL(fn_aux);
		if(::hts_set_fai_filename(get(),fn_aux)!=0) {
			THROW_ERROR("Cannot set reference");
			}
		}

std::unique_ptr<HtsFile> HtsFile::open(const char* filename,const char* mode) {
   htsFile* fp;
   errno = 0;
   std::unique_ptr<HtsFile> p;
   if((fp = ::hts_open((filename==NULL?"-":filename),mode))==NULL) {
		return p;
		}
	p.reset(new HtsFile(fp,true));
	return p;
	}

std::unique_ptr<HtsFile> HtsFile::wrap(htsFile* pt) {
	unique_ptr<HtsFile> p(new HtsFile(pt,false));
	return p;
	}
