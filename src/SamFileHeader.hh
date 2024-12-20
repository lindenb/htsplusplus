#ifndef SAM_FILE_HEADER_HH
#define SAM_FILE_HEADER_HH

#include <set>
#include <memory>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include "SamSequenceDictionary.hh"
#include "utils.hh"

namespace htspp {

class SamFileHeader:public Pointer<sam_hdr_t> {
  private:
  	mutable std::shared_ptr<SamSequenceDictionary> _dict;
	protected:
		SamFileHeader( sam_hdr_t *header);
	public:
		virtual ~SamFileHeader();
		virtual void write(htsFile* fp);
		virtual const char* tid2name(int tid) const;
		virtual std::unique_ptr<std::string> find_tag_hd(const char* tag);
		virtual std::unique_ptr<std::string> sort_order();
		virtual bool has_sort_order(const char* s);
		virtual void assert_sort_order(const char* s);
		virtual bool is_coordinate_sorted() ;
		virtual void assert_coordinate_sorted();
		virtual std::set<std::string> samples();
		virtual std::shared_ptr<SamSequenceDictionary> dictionary() const;

		static std::unique_ptr<SamFileHeader> read(samFile *fp,int flag);
		static std::unique_ptr<SamFileHeader> wrap(sam_hdr_t *header);
	};

}

#endif

