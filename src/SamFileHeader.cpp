#include <memory>
#include "SamFileHeader.hh"
#include "debug.hh"

using namespace std;
using namespace htspp;

SamFileHeader::SamFileHeader( sam_hdr_t *header):Pointer(header) {
			}

SamFileHeader::~SamFileHeader() {
			}

void SamFileHeader::write(htsFile* fp) {
		if ( fp==NULL || this->ptr==NULL || ::sam_hdr_write(fp, get())!=0 ) {
			THROW_ERROR("Cannot write SAM header");
			}
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

const char* SamFileHeader::tid2name(int tid) const {
	return ::sam_hdr_tid2name(get(), tid);
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

class SamFileHeader2 : public SamFileHeader {
    bool free_on_close;
    public:
        SamFileHeader2(sam_hdr_t* h,bool free_on_close):SamFileHeader(h),free_on_close(free_on_close) {}
        virtual ~SamFileHeader2() {
            if(ptr!=NULL && free_on_close) ::sam_hdr_destroy(ptr);
            }
    };


std::unique_ptr<SamFileHeader> SamFileHeader::read(samFile *fp,int flags) {
			sam_hdr_t *header = ::sam_hdr_read(fp);
			return std::unique_ptr<SamFileHeader>(header==NULL?NULL:new SamFileHeader2(header, true));
			}


std::unique_ptr<SamFileHeader> SamFileHeader::wrap(sam_hdr_t* header) {
			return std::unique_ptr<SamFileHeader>(header==NULL?NULL:new SamFileHeader2(header,false));
			}

