#include "Faidx.hh"

using namespace std;
using namespace htspp;

Faidx::Faidx(faidx_t *fai):Pointer(fai) {
		}

Faidx::~Faidx() {
	if(get()!=NULL) ::fai_destroy(get());
	}

std::unique_ptr<Faidx> Faidx::load(const char* filename) {
	ASSERT_NOT_NULL(filename);
	faidx_t *f = fai_load(filename);
	ASSERT_NOT_NULL(f);
	return std::unique_ptr<Faidx>(new Faidx(f));
	}
	
	int Faidx::nseq() const {
			return ::faidx_nseq(get());
			}
	const char* Faidx::name(int i) const {
		return ::faidx_iseq(get(),i);
		}
hts_pos_t Faidx::length(const char *reg) const {
	return ::faidx_seq_len64(get(),reg);
	}


std::shared_ptr<SamSequenceDictionary> Faidx::dictionary()  const {
	if(_dict.get()==NULL) {
		_dict.reset( SamSequenceDictionary::of_faidx(get()).release());
	}
	return _dict;
}

std::unique_ptr<std::string> Faidx::fetch( const char *c_name, hts_pos_t p_beg_i, hts_pos_t p_end_i) const {
			ASSERT_NOT_NULL(c_name);
			hts_pos_t len=0;
			char* s = ::faidx_fetch_qual64(get(),c_name,p_beg_i,p_end_i,&len);
			if(s==NULL) {
				if(len==-2) {
					//seq not present
					return std::unique_ptr<std::string>();
					}
				else
					{
					THROW_ERROR("cannot fecth");
					}
				}
			std::string* str=new std::string(s,(std::string::size_type)len);
			std::free(s);
			return std::unique_ptr<std::string>(str);
			}