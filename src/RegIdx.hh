#ifndef REGIDX_HH
#define REGIDX_HH
#include <htslib/regidx.h>
#include <memory>
#include "debug.hh"
#include "utils.hh"
/** basic wrapper for regidx_t */


class RegIdx {
	public:
		regidx_t *idx;
		typedef int (*callback1)(const char* chr, int querybeg, int queryend, int beg, int end,void* usr);
		typedef int (*callback2)(const char* chr, int beg, int end,void* usr);
		regidx_t* get() {
			return idx;
			}
	protected:
		/** private use RegIdxFactory */
		RegIdx():idx(NULL) {
			}
		
		class RegItr : public Pointer<regitr_t> {
			public:
				RegItr(regitr_t* itr):Pointer(itr) {
					ASSERT_NOT_NULL(itr);
					}
				
				virtual ~RegItr()
					{
					if(ptr!=NULL) ::regitr_destroy(ptr);
					}
				bool overlap() {
					return ::regitr_overlap(get());
					}
				bool loop() {
					return ::regitr_loop(get());
					}
			};
		std::unique_ptr<RegItr> make_iterator() {
			return std::unique_ptr<RegItr>(new RegItr(::regitr_init(this->get())));
			}
	public:

		virtual ~RegIdx() {
			if(idx!=NULL) ::regidx_destroy(idx);
			}
		bool overlaps(const char* chr, int beg1, int end1) {
			ASSERT_NOT_NULL(chr);
			std::unique_ptr<RegItr> itr = make_iterator();
			return ::regidx_overlap(idx, chr,beg1-1,end1-1 /** what ?*/, itr->get());
			}

		virtual unsigned int count(const char* chr, int beg1, int end1) {
			unsigned int n = 0UL;
			std::unique_ptr<RegItr> itr = make_iterator();
			if( ::regidx_overlap(get(), chr,beg1-1,end1-1, itr->get())) {
            while ( itr->overlap() ) {
					++n;
                	}
                	}
			return n;
			}

		virtual unsigned int count() {
			unsigned int  n=0UL;
			std::unique_ptr<RegItr> itr = make_iterator();
        	while ( itr->loop() ) {
				++n;
               	}
			return n;
			}

		virtual void for_each(RegIdx::callback2 fun,void* usr) {
			regitr_t *itr = ::regitr_init(idx);
			ASSERT_NOT_NULL(itr);
			while ( ::regitr_loop(itr) ) {
				int ret = fun(itr->seq,(itr->beg)+1,(itr->end)+1,usr);
				if(ret!=0) break;
				}
	        	::regitr_destroy(itr);
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
