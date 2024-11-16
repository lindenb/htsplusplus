#include <cassert>
#include <sstream>

#include "SamRecord.hh"

using namespace htspp;
using namespace std;

SamRecord& SamRecord::operator=(const SamRecord& cp) {
if(this!=&cp) {
    ::bam_destroy1(this->b);
    this->b=::bam_dup1(cp.b);
    assert(b!=NULL);
    this->header = cp.header;
    }
return *this;
}


		bam1_t* SamRecord::get() {return b;}
		bam1_t* SamRecord::get() const {return b;}


		SamRecord::SamRecord():b(::bam_init1()),header(NULL) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}

			}
		SamRecord::SamRecord(SamFileHeader* header):b(::bam_init1()),header(header) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}

			}
		SamRecord::SamRecord(const SamRecord& cp):b(::bam_dup1(cp.b)),header(cp.header) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}
			}

	 SamRecord::~SamRecord() {
			if(b!=NULL) ::bam_destroy1(b);
			}

       const char* SamRecord::contig() const {return header->tid2name(tid());}
       hts_pos_t SamRecord::start() const { return AbstractSamRecord::start();}
       hts_pos_t SamRecord::end() const { return AbstractSamRecord::end();}


		std::unique_ptr<SamRecord> SamRecord::clone() {
			return std::unique_ptr<SamRecord>(new SamRecord(*this));
			}


		std::ostream& SamRecord::print(std::ostream& out) const {
			KString str;
			if (sam_format1(header->get(), get(), str.get()) < 0);
			return str.print(out);
			}
		std::string SamRecord::to_string() const {
			ostringstream os;
			print(os);
			return os.str();
			}
