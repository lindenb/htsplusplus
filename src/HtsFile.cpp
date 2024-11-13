#include "debug.hh"
#include "HtsFile.hh"


HtsFile::HtsFile( htsFile *fp):fp(fp) {
			}

int HtsFile::set_opt(hts_fmt_option opcode,int value) {
	 return ::hts_set_opt(this->fp, opcode, value);
	}

void HtsFile::close() {
		if(fp!=NULL) {
			::hts_close(fp);
			fp = NULL;
			}
		}
bool HtsFile::is_open() {
			return fp!=NULL;
			}

HtsFile::~HtsFile() {
			close();
			}
htsFile* HtsFile::get() {
	return fp;
	}

htsFile* HtsFile::operator()() {
	return get();
	}

void HtsFile::set_reference(const char *fn_aux) {
		ASSERT_NOT_NULL(fn_aux);
		if(::hts_set_fai_filename(get(),fn_aux)!=0) {
			THROW_ERROR("Cannot set reference");
			}
		}

HtsFile* HtsFile::open(const char* filename,const char* mode) {
	 htsFile* fp;
   errno = 0;
   if((fp = ::hts_open((filename==NULL?"-":filename),mode))==NULL) {
		return NULL;
		}
	return new HtsFile(fp);
	}
