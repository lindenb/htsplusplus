#include "BcfReader.hh"

std::unique_ptr<BcfReader> BcfReader::open(const char* filename) {
	std::unique_ptr<HtsFile> fp = HtsFile::open(filename,"r");
	std::unique_ptr<BcfHeader> header = BcfHeader::read(fp->get());
	BcfReader*  r = new BcfReader;
	r->fp.swap(fp);
	r->header.swap(header);
	return std::unique_ptr<BcfReader>(r);
	}
