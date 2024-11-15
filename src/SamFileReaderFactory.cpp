#include "SamFileReaderFactory.hh"

using namespace std;
using namespace htspp;

SamFileReader* SamFileReaderFactory::createNewSamFileReader() {
	    return new SamFileReader;
	    }
SamFileReaderFactory::SamFileReaderFactory():threadPool(NULL),requires_index(false) {
			}

SamFileReaderFactory& SamFileReaderFactory::threads(HtsThreadPool* threadPool) {
			this->threadPool = threadPool;
			return *this;
			}

SamFileReaderFactory& SamFileReaderFactory::load_index(bool b) {
		this->requires_index = b;
		return *this;
		}

SamFileReaderFactory& SamFileReaderFactory::reference(const char* fasta) {
		this->_reference.assign(fasta);
		return *this;
		}

		
SamFileReaderFactory::~SamFileReaderFactory() {
  }

unique_ptr<SamFileReader> SamFileReaderFactory::open(const char* filename,const char* bai) {
      unique_ptr<SamFileReader> sfr(createNewSamFileReader());
      sfr->fp = HtsFile::open(filename, "rb");
		    if (!(sfr->fp)) {
		       THROW_ERROR("Cannot open " << filename);
		    	}
		   
		    if (!sfr->fp->set_opt(CRAM_OPT_DECODE_MD, 0)) {
		        THROW_ERROR("Failed to set CRAM_OPT_DECODE_MD value.");
		   		}
			if(threadPool!=NULL) threadPool->bind(sfr->fp->fp);
			sfr->header = SamFileHeader::read(sfr->fp->fp,0);
			if(!sfr->header) {
			     THROW_ERROR("Cannot read header");
					goto error;
					}
			
			if(this->requires_index) {
				sfr->idx = HtsIndex::load(filename,0,0);
				if(!sfr->idx) {
					THROW_ERROR("Cannot load index for " << filename);
					goto error;
					}
				}
		    if(!_reference.empty()) {
            if (hts_set_fai_filename(sfr->fp->get(),_reference.c_str()) != 0) {
  			
                }
            }
			
			  return sfr;

			error:
			unique_ptr<SamFileReader> p2;
			  return p2;
			}
