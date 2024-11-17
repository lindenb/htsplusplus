#ifndef HTSFILE_HPP
#define HTSFILE_HPP

#include <cerrno>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <htslib/hts.h>
#include "utils.hh"
#include "debug.hh"

namespace htspp {

class HtsFile : public Pointer<htsFile> {
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
		static std::unique_ptr<HtsFile> open(const char* fn,const char* mode);
		static std::unique_ptr<HtsFile> open(const char* fn);
		static std::unique_ptr<HtsFile> wrap(htsFile* p);
	};

}

#endif

