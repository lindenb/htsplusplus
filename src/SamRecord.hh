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

		bam1_t* get() const;
		bam1_t* get();

		SamRecord();
		SamRecord(SamFileHeader* header);

	
		SamRecord(const SamRecord& cp);

		virtual ~SamRecord();

    virtual const char* contig() const;
    virtual hts_pos_t start() const;
    virtual hts_pos_t end() const;
		virtual std::unique_ptr<SamRecord> clone();
		SamRecord& operator=(const SamRecord& cp);
		virtual std::ostream& print(std::ostream& out) const ;
		virtual std::string to_string() const;
		};



}

#endif

