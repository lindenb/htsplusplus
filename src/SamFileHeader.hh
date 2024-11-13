#ifndef SAM_FILE_HEADER_HH
#define SAM_FILE_HEADER_HH
#include <set>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include "HtsFile.hh"
#include "utils.hh"
#include "debug.hh"

class SamFileHeader {
	public:
		sam_hdr_t* header;
		SamFileHeader( sam_hdr_t *header);
		virtual ~SamFileHeader();
		virtual void write(htsFile* fp);
		virtual sam_hdr_t* get();
		virtual sam_hdr_t* operator()();
		virtual const char* tid2name(int tid);
		virtual std::unique_ptr<std::string> find_tag_hd(const char* tag);
		virtual std::unique_ptr<std::string> sort_order();
		virtual bool has_sort_order(const char* s);
		virtual void assert_sort_order(const char* s);
		virtual bool is_coordinate_sorted() ;
		virtual void assert_coordinate_sorted();
		virtual std::set<std::string> samples();
		static SamFileHeader* read(samFile *fp,int flag);
	};

#endif

