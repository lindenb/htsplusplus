#ifndef SAMRECORD_HPP
#define SAMRECORD_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "SamFileHeader.hh"

class AbstractSamRecord {
	public:
		virtual bam1_t* get()=0;
		virtual bam1_t* get_not_null() {
			bam1_t* b = get();
			ASSERT_NOT_NULL(b);
			return b;
			}
		AbstractSamRecord() {
			}
		virtual ~AbstractSamRecord() {
			}
		virtual int flags() {
			return get_not_null()->core.flag;
			}
		virtual bool has_flag(int flg) {
			return flags() & flg;
			}
		virtual int tid() {
			return get_not_null()->core.tid;
			}		
		virtual int start0() {
			return get_not_null()->core.pos;
			}
		virtual int start() {
			return start0() + 1;
			}
		virtual int end() {
			return bam_endpos(get_not_null());
			}
		virtual const char* read_name() {
			return bam_get_qname(get());
			}
		virtual bool is_unmapped() {
			return has_flag(BAM_FUNMAP);
			}
        virtual bool is_mate_unmapped() {
			return has_flag(BAM_FMUNMAP);
			}
         virtual bool is_paired() {
			return has_flag(BAM_FPAIRED);
			}
         virtual bool is_first_of_pair() {
			return has_flag(BAM_FREAD1);
			}
         virtual bool is_second_of_pair() {
			return has_flag(BAM_FREAD2);
			}
        virtual bool is_qc_fail() { return has_flag(BAM_FQCFAIL);}
        virtual bool is_duplicate() { return has_flag(BAM_FDUP);}
        virtual bool is_reverse_strand() { return has_flag(BAM_FREVERSE);}
        virtual bool is_mate_reverse_strand() { return has_flag(BAM_FMREVERSE);}
        virtual bool is_proper_pair() { return has_flag(BAM_FPROPER_PAIR);}
        virtual bool is_secondary() { return has_flag(BAM_FSECONDARY);}
        virtual bool is_supplementary() { return has_flag(BAM_FSUPPLEMENTARY);}
        virtual bool is_secondary_or_supplementary() { return is_secondary() || is_supplementary() ;}

        /** java htsdk functions */
        bool getReadUnmappedFlag() {return is_unmapped();}
        bool getMateUnmappedFlag() {return is_mate_unmapped();}
        bool getReadPairedFlag() { return is_paired();}
        bool getFirstOfPairFlag() { return is_first_of_pair();}
        bool getSecondOfPairFlag() { return is_second_of_pair();}
        bool isSecondaryOrSupplementary() { return is_secondary_or_supplementary();}
        int getFlags() { return flags();}
        const char* getReadName() { return read_name();}
	};


class SamRecord : public AbstractSamRecord {
	public:
		bam1_t* b;

		SamRecord():b(::bam_init1()) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}

			}
		SamRecord(const SamRecord& cp):b(::bam_dup1(cp.b)) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}
			}
		virtual bam1_t* get() {
			return b;
			}
		virtual ~SamRecord() {
			if(b!=NULL) ::bam_destroy1(b);
			}
		std::unique_ptr<SamRecord> clone() {
			return std::unique_ptr<SamRecord>(new SamRecord(*this));
			}
		};
#endif

