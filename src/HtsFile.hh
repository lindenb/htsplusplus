#ifndef HTSFILE_HPP
#define HTSFILE_HPP

#include <cerrno>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <htslib/hts.h>
#include "debug.hh"

class HtsFile {
	protected:
		HtsFile( htsFile *fp);
	public:
		htsFile *fp;
		virtual void close();
		virtual bool is_open();
		virtual ~HtsFile();
		virtual htsFile* get();
		virtual htsFile* operator()();
		virtual void set_reference(const char *fn_aux);
		virtual int set_opt(hts_fmt_option key,int value);
		static HtsFile* open(const char* fn,const char* mode);
	};

#endif

