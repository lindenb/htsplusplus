#ifndef SAMFILE_READER_FACTORY_HPP
#define SAMFILE_READER_FACTORY_HPP

#include <cerrno>
#include "debug.hh"
#include "HtsThreadPool.hh"
#include "HtsIdx.hh"
#include "SamRecord.hh"
#include "SamFileReader.hh"
#include "Coverage.hh"


class SamFileReaderFactory {
	private:
		HtsThreadPool* threadPool;
		std::string reference;
		bool requires_index;
	protected:
	  virtual SamFileReader* createNewSamFileReader();
	public:
		SamFileReaderFactory();
		virtual ~SamFileReaderFactory();
		SamFileReaderFactory& threads(HtsThreadPool* threadPool);
		SamFileReaderFactory& load_index(bool b);
		SamFileReader* open(const char* filename,int flags);
	};



#endif

