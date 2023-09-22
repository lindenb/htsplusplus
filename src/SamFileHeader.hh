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

		SamFileHeader( sam_hdr_t *header):header(header) {
			}

		virtual ~SamFileHeader() {
			if(header!=NULL) ::sam_hdr_destroy(header);
			}
		void write(htsFile* fp) {
			if ( fp==NULL || header==NULL || ::sam_hdr_write(fp, header)!=0 ) {
				THROW_ERROR("Cannot write SAM header");
				}
			}
		sam_hdr_t* get() {
			return header;
			}
		sam_hdr_t* operator()() {
			return get();
			}
		const char* tid2name(int tid) {
			return ::sam_hdr_tid2name(get(),tid);
			}
		/*  Returns the value associated with a given '@HD' line tag */
		virtual std::unique_ptr<std::string> find_tag_hd(const char* tag)
			{
			std::string* ret = NULL;
			ASSERT_NOT_NULL(tag);
			kstring_t str = KS_INITIALIZE;
			if(sam_hdr_find_tag_hd(get(),tag,&str) == 0 && str.s!=0) {
				ret = new std::string(str.s);
				}
    		ks_free(&str);
    		return std::unique_ptr<std::string>(ret);
    		}
    	/** retrieve sort order */
		virtual std::unique_ptr<std::string> sort_order() {
			return find_tag_hd("SO");
			}
		virtual bool has_sort_order(const char* s) {
			std::unique_ptr<std::string> p = sort_order();
			if(!p) return false;
			return p->compare(s)==0;
			}
		virtual void assert_sort_order(const char* s) {
			ASSERT_NOT_NULL(s);
			std::unique_ptr<std::string> p = sort_order();
			if(!p) THROW_ERROR("cannot get SO (sort order) in SAM header");
			if(p->compare(s)!=0) {
				THROW_ERROR("expected sort order SO="<< s << "but got SO=" << *(p) << " in SAM header");
				}
			}
		virtual bool is_coordinate_sorted() {
			return has_sort_order("coordinate");
			}
		virtual void assert_coordinate_sorted() {
			assert_sort_order("coordinate");
			}
	
		virtual std::set<std::string> samples() {
				std::set<std::string> _samples;
				/* get the RG lines */
				int n_rg = ::sam_hdr_count_lines(get(), "RG");
				kstring_t sm_val = KS_INITIALIZE;
				for (int i = 0; i < n_rg; i++) {
				    int r = ::sam_hdr_find_tag_pos(get(), "RG", i, "SM", &sm_val);
				    if (r < 0) continue;
				    std::string sample(ks_str(&sm_val));
				    _samples.insert(sample);    
					}
				ks_free(&sm_val);
				return _samples;
				}

	
	
		static std::unique_ptr<SamFileHeader> read(samFile *fp);
	};

#endif

