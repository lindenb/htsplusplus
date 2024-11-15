#ifndef SAMRECORD_HPP
#define SAMRECORD_HPP

#include <cerrno>
#include "debug.hh"
#include "Locatable.hh"
#include "AbstractSamRecord.hh"
#include "KString.hh"
namespace htspp {


class SamRecord : public Locatable, public AbstractSamRecord {
	public:
		bam1_t* b;
		SamFileHeader* header;

		bam1_t* get() {return b;}
		bam1_t* get() const {return b;}

		SamRecord():b(::bam_init1()),header(NULL) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}

			}
		SamRecord(SamFileHeader* header):b(::bam_init1()),header(header) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}

			}
		SamRecord(const SamRecord& cp):b(::bam_dup1(cp.b)),header(cp.header) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}
			}

		virtual ~SamRecord() {
			if(b!=NULL) ::bam_destroy1(b);
			}

                virtual const char* contig() const;
                virtual hts_pos_t start() const { return AbstractSamRecord::start();}
                virtual hts_pos_t end() const;


		std::unique_ptr<SamRecord> clone() {
			return std::unique_ptr<SamRecord>(new SamRecord(*this));
			}

		SamRecord& operator=(const SamRecord& cp);

		virtual std::ostream& print(std::ostream& out) const {
			KString str;
			if (sam_format1(header->get(), get(), str.get()) < 0);
			return str.print(out);
			}
		};



}

#endif

