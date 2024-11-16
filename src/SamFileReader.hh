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
		bool has_index();
		bool read1(bam1_t* b);
		bool read2(SamRecord* rec);
		std::unique_ptr<SamRecord> read2();
		std::unique_ptr<SamRecordIterator> query(const Locatable& loc);
		std::unique_ptr<SamRecordIterator> query(const char* reg);
		std::unique_ptr<SamRecordIterator> iterator();
	};

}

#endif

