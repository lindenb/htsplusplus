#ifndef HTSFILE_HPP
#define HTSFILE_HPP

#include <cerrno>
#include <stdexcept>
#include <sstream>
#include <memory>
#include <htslib/hts.h>
#include "utils.hh"
#include "debug.hh"

class HtsFile : public Pointer<htsFile> {
	private:
		bool close_in_descructor;
	protected:
		HtsFile( htsFile *fp,bool close_in_descructor);
	public:
		htsFile *fp;
		virtual void close();
		virtual bool is_open();
		virtual ~HtsFile();
		virtual htsFile* operator()();
		virtual void set_reference(const char *fn_aux);
		virtual int set_opt(hts_fmt_option key,int value);
		static std::unique_ptr<HtsFile> open(const char* fn,const char* mode);
		static std::unique_ptr<HtsFile> wrap(htsFile* p);
	};

#endif

