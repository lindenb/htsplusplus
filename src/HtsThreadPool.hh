#ifndef HTS_THREAD_POOL_HH
#define HTS_THREAD_POOL_HH
#include <htslib/hts.h>
#include <htslib/thread_pool.h>
#include <memory>
#include "debug.hh"

class HtsThreadPool  {
	public:
		htsThreadPool pool;
	private:
		HtsThreadPool(int nthreads) {
			if(nthreads>0) {
				pool.pool = ::hts_tpool_init(nthreads);
				if(pool.pool==NULL) {
					THROW_ERROR("cannot create "<<nthreads<< "threads");
					}
				}
			else
				{
				pool.pool = NULL;
				}
			}
	 public:
		virtual ~HtsThreadPool() {
			if(this->pool.pool!=NULL) {
			 	::hts_tpool_destroy(this->pool.pool);
			 	}
			}
		virtual void bind( htsFile* htsFile) {
			if(this->pool.pool!=NULL && htsFile!=NULL) {
				hts_set_opt(htsFile,  HTS_OPT_THREAD_POOL, &pool);
				}
			}
		static std::unique_ptr<HtsThreadPool> create(int nthreads);
	};

#endif
