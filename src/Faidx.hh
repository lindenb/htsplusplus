#ifndef FAIDX_HH
#define FAIDX_HH
#include <htslib/faidx.h>
#include <memory>
#include "Locatable.hh"
#include "debug.hh"
#include "utils.hh"

class Faidx {
	public:
		faidx_t *fai;
	protected:
		Faidx(faidx_t *fai):fai(fai) {
			}
		class Contig : public Locatable {
			Faidx* owner;
			int idx;
			public:
			Contig(Faidx* owner,int idx):owner(owner),idx(idx) {
				}
			Contig(const Contig& cp):owner(cp.owner),idx(cp.idx) {
				}
			virtual ~Contig() {
				}
			virtual const char* contig() const {
				return owner->name(idx);
				}
			virtual hts_pos_t start() const {
				return 1;
				}
			virtual hts_pos_t end() const {
				return 0;
				}
			};
	public:
		faidx_t* get() {
			return fai;
			}
		virtual ~Faidx() {
			if(fai!=NULL) ::fai_destroy(fai);
			}
		hts_pos_t length(const char *reg) {
			return ::faidx_seq_len64(get(),reg);
			}
		std::unique_ptr<Locatable> get(int idx) {
			return std::unique_ptr<Locatable>(new Contig(this,idx));
			}

		std::unique_ptr<std::string> fetch( const char *c_name, hts_pos_t p_beg_i, hts_pos_t p_end_i) {
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
		int nseq() {
			return ::faidx_nseq(get());
			}
		const char* name(int i) {
			return ::faidx_iseq(get(),i);
			}
		
		static std::unique_ptr<Faidx> load(const char* filename);
	};

#endif
