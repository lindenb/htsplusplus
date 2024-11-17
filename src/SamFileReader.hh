#ifndef SAMFILE_HPP
#define SAMFILE_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "SamRecord.hh"
#include "SamFileHeader.hh"
#include "SamRecordIterator.hh"
#include "Coverage.hh"
#include "HtsFile.hh"
#include "HtsIdx.hh"

namespace htspp {


class SamFileReader {
	public:
		typedef int (*callback1_t)(SamRecord*,void* data);
		std::unique_ptr<HtsFile> fp;
		std::unique_ptr<SamFileHeader> header;
		//associated index
		std::unique_ptr<HtsIndex> idx;
		SamFileReader();
		virtual ~SamFileReader();
		virtual bool has_index();
		virtual bool read1(bam1_t* b);
		virtual bool read2(SamRecord* rec);
		virtual std::unique_ptr<SamRecord> read2();
		virtual std::unique_ptr<SamRecordIterator> query_by_loc(const Locatable* loc);
		virtual std::unique_ptr<SamRecordIterator> query(const char* reg);
		virtual std::unique_ptr<SamRecordIterator> iterator();
		virtual std::shared_ptr<SamSequenceDictionary> dictionary() const;
	};

}

#endif

