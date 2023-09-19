#ifndef TABIX_HH
#define TABIX_HH
#include <htslib/tbx.h>

class Tabix {
	public:
        tbx_t *tbx;
	protected:
		Tabix( tbx_t *tbx)	:tbx(tbx) {
			}
        std::string str(const char* contig,hts_pos_t beg, hts_pot_t end) {
            std::ostringstream os;
            os << contig << ":" << beg << "-" << end;
            return os.str();
            }
	public:
        tbx_t* get() {
            return tbx;
            } 
		virtual ~Tabix() {
            if(tbx!=NULL) ::tbx_destroy(tbx);
			}
        virtual int name2id(const char *ss) {
            return int tbx_name2id(get(),ss);            
            }  
        virtual void foreach(const char* contig, hts_pos_t beg, hts_pot_t end, callback_t fun,(void*)usr) {
            std::string rgn =  str(contig, beg,  end);
            hts_itr_t *itr = NULL;
            kstring_t s;
            if ((itr = tbx_itr_querys(begin(), s)) == 0) continue;
            s.s = 0; s.l = s.m = 0;
            while (tbx_bgzf_itr_next(fp, tbx, itr, &s) >= 0) {
                int ret=fun(s,usr);
                if(ret!=0) break;
                }
            tbx_itr_destroy(itr);
            free(s.s);
            }
         virtual unsigned long count(const char* contig, hts_pos_t beg, hts_pot_t end) {
            unsigned long n=0UL;
            this->foreach( mem_fun( &MyClass::Bar ),&n);
            return n;
            }
        virtual bool overlap(const char* region) {
            }
	static std::unique_ptr<Tabix> load(const char* fname);
	};

#endif

