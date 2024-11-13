#include <memory>
#include "SamFileHeader.hh"
#include "debug.hh"



SamFileHeader::SamFileHeader( sam_hdr_t *header):header(header) {
			}

SamFileHeader::~SamFileHeader() {
			if(header!=NULL) ::sam_hdr_destroy(header);
			}
	void SamFileHeader::write(htsFile* fp) {
		if ( fp==NULL || header==NULL || ::sam_hdr_write(fp, header)!=0 ) {
			THROW_ERROR("Cannot write SAM header");
			}
		}
		sam_hdr_t* SamFileHeader::get() {
			return header;
			}
		sam_hdr_t* SamFileHeader::operator()() {
			return get();
			}
		const char* SamFileHeader::tid2name(int tid) {
			return ::sam_hdr_tid2name(get(),tid);
			}
		/*  Returns the value associated with a given '@HD' line tag */
std::unique_ptr<std::string> SamFileHeader::find_tag_hd(const char* tag)
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
std::unique_ptr<std::string> SamFileHeader::sort_order() {
			return find_tag_hd("SO");
			}
bool SamFileHeader::has_sort_order(const char* s) {
			std::unique_ptr<std::string> p = sort_order();
			if(!p) return false;
			return p->compare(s)==0;
			}
void SamFileHeader::assert_sort_order(const char* s) {
			ASSERT_NOT_NULL(s);
			std::unique_ptr<std::string> p = sort_order();
			if(!p) THROW_ERROR("cannot get SO (sort order) in SAM header");
			if(p->compare(s)!=0) {
				THROW_ERROR("expected sort order SO="<< s << "but got SO=" << *(p) << " in SAM header");
				}
			}
bool SamFileHeader::is_coordinate_sorted() {
			return has_sort_order("coordinate");
			}
void SamFileHeader::assert_coordinate_sorted() {
			assert_sort_order("coordinate");
			}
	
std::set<std::string> SamFileHeader::samples() {
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



SamFileHeader* SamFileHeader::read(samFile *fp,int flags) {
			sam_hdr_t *header = ::sam_hdr_read(fp);
			return header==NULL?NULL:new SamFileHeader(header);
			}

