#include <htslib/sam.h>
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

void HtsFile::fush() {
	if(!is_null()) ::hts_flush(get());
	}
const htsFormat* HtsFile::format() {
	return ::hts_get_format(get());
}

void HtsFile::set_cache_size(int n) {
	hts_set_cache_size(get(),n);
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

std::unique_ptr<HtsFile> HtsFile::open(const char* filename) {
	return open(filename,"rb");
	}

std::unique_ptr<HtsFile> HtsFile::wrap(htsFile* pt) {
	unique_ptr<HtsFile> p(new HtsFile(pt,false));
	return p;
	}



std::unique_ptr<HtsIndex> HtsFile::load_sam_index(const char* bam) {
	return load_sam_index(bam,NULL);
	}

std::unique_ptr<HtsIndex> HtsFile::load_sam_index(const char* bam,const char* bai) {
	hts_idx_t* i = ::sam_index_load2(get(), bam,bai);
 	if(i==NULL) THROW_ERROR("cannot load index for " << bam);
	return HtsIndex::of(i);
	}
	
