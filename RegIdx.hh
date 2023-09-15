#ifndef REGIDX_HH
#define REGIDX_HH
#include <htslib/regidx.h>
#include <memory>

/** basic wrapper for regidx_t */
class RegIdx {
	public:
		regidx_t *idx;
		typedef int (*callback1)(const char* chr, int querybeg, int queryend, int beg, int end);
		typedef int (*callback2)(const char* chr, int beg, int end);
	protected:
		/** private use RegIdxFactory */
		RegIdx():idx(NULL) {
			}
	public:

		virtual ~RegIdx() {
			if(idx!=NULL) ::regidx_destroy(idx);
			}
		bool overlaps(const char* chr, int beg, int end) {
		        regitr_t *itr = ::regitr_init(idx);
        		bool found = regidx_overlap(idx, chr,beg-1,end-1, itr);
	        	::regidx_destroy(idx);
			return found;
			}

		virtual unsigned int count(const char* chr, int beg, int end) {
			unsigned int n = 0UL;
			regitr_t *itr = ::regitr_init(idx);
                         while ( regitr_overlap(itr) ) {
				++n;
                                }
                        ::regidx_destroy(idx);
			return n;
			}

		virtual unsigned int count() {
			unsigned int n = 0UL;
			regitr_t *itr = ::regitr_init(idx);
                        while ( regitr_loop(itr) ) {
				++n;
                        	}
                        ::regidx_destroy(idx);
			return n;
			}

		virtual void for_each(RegIdx::callback2 fun) {
			regitr_t *itr = ::regitr_init(idx);
			while ( ::regitr_loop(itr) ) {
				int ret = fun(itr->seq,itr->beg+1, itr->end+1);
				if(ret!=0) break;
				}
	        	::regidx_destroy(idx);
			}
		static std::unique_ptr<RegIdx> load_file(const char* filename);

		};


#ifdef XXX

template<class T>
class ExtendedRegIdx<T>:public RegIdx {
	private:
		/** private use RegIdxFactory */
		ExtendedRegIdx():RegIdx() {
			}
		class Accumulator {
			public:
				std::vector<T*>& array;
				Accumulator(std::vector<T*>& array):array(array) {
					}
				int operator()(const char* chr, int querybeg, int queryend, int beg, int end,T* tem ) {
					array.push_back(item);
					return 0;
					}
			};
	public:

		virtual ~ExtendedIdx() {
			}

	};


template<class T>
class RegIdxFactory {
	private:
		regidx_parse_f parsef;
		regidx_free_f freef;
	public:
		RegIdxFactory( regidx_parse_f parsef, regidx_free_f freef):parsef(parsef),freef(freef) {
			}
		RegIdxFactory():parsef(NULL),freef(NULL) {
			}
		virtual ~RegIdxFactory() {
			}
	public:
		std::unique_ptr<ExtendedRegIdx<T> > load_file(const char* filename) {
			if(in_fname==NULL) {
			THROW_ERROR("filename is NULL");
			}
		RegIdx* ptr = new RegIdx;
		ptr->idx = ::regidx_init(in_fname, this->parsef , this->freef ,0,(void*)this);
		if(ptr->idx==NULL) {
			delete ptr;
			THROW_ERROR("cannot load " << in_fname);
			}
		return std::unique_ptr<RegIdx>(ptr);
		}
	};

#endif

#endif
