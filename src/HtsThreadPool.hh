/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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
					THROW_ERROR("cannot create "<<nthreads<< " threads");
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
