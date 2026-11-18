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

#ifndef HTSFILE_HPP
#define HTSFILE_HPP

#include <cerrno>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <htslib/hts.h>
#include "utils.hh"
#include "debug.hh"
#include "HtsIdx.hh"
namespace htspp {

class HtsFile : public Pointer<htsFile> {
	public:
		enum {
			THROW_ON_ERROR = 0x01,    /* First bit set */
			VERBOSE = 0x02,    /* First bit set */
			//,DRAW_REPEAT_Y = 0x02,    /* Second bit set */
			//DRAW_MIRRORED = 0x04,    /* Third bit set */
			};

	private:
		bool close_in_descructor;
	protected:
		HtsFile( htsFile *fp,bool close_in_descructor);
	public:
		virtual void close();
		virtual bool is_open();
		virtual ~HtsFile();
		virtual const htsFormat *format();
		virtual void set_reference(const char *fn_aux);
		virtual int set_opt(hts_fmt_option key,int value);
		virtual void fush();
		virtual void set_cache_size(int n);
		static std::shared_ptr<HtsFile> open(const char* fn,const char* mode,int modifiers);
		static std::shared_ptr<HtsFile> open(const char* fn);
		static std::shared_ptr<HtsFile> wrap(htsFile* p);
	};

}

#endif

