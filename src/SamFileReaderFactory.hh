#ifndef SAMFILE_READER_FACTORY_HPP
#define SAMFILE_READER_FACTORY_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "HtsIdx.hh"
#include "SamRecord.hh"
#include "SamFileReader.hh"
#include "Coverage.hh"

namespace htspp {

class SamFileReaderFactory {
	private:
		HtsThreadPool* threadPool;
		std::string _reference;
	protected:
	  virtual SamFileReader* createNewSamFileReader();
	public:
		SamFileReaderFactory();
		virtual ~SamFileReaderFactory();
		virtual SamFileReaderFactory& threads(HtsThreadPool* threadPool);
		virtual SamFileReaderFactory& reference(const char* fasta);
		virtual std::unique_ptr<SamFileReader> open();//stdin
		virtual std::unique_ptr<SamFileReader> open(const char* filename);
		virtual std::unique_ptr<SamFileReader> open(const char* filename,bool require_index);
		virtual std::unique_ptr<SamFileReader> open(const char* filename,const char* index_or_NULL,bool require_index);
	};

}


#endif

