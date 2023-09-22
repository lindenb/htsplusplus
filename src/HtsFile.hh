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
		HtsFile( htsFile *fp):fp(fp) {
			}
	public:
		htsFile *fp;
		virtual void close() {
			if(fp!=NULL) {
				::hts_close(fp);
				fp = NULL;
				}
			}
		virtual bool is_open() {
			return fp!=NULL;
			}
		virtual ~HtsFile() {
			close();
			}
		htsFile* get() {
			return fp;
			}
		htsFile* operator()() {
			return get();
			}
		virtual void set_reference(const char *fn_aux) {
			ASSERT_NOT_NULL(fn_aux);
			if(::hts_set_fai_filename(get(),fn_aux)!=0) {
				THROW_ERROR("Cannot set reference");
				}
			}

		static std::unique_ptr<HtsFile> open(const char* fn,const char* mode);
	};

#endif

