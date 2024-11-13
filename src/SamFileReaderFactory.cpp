#include "SamFileReaderFactory.hh"


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
		
SamFileReaderFactory::~SamFileReaderFactory() {
  }

SamFileReader* SamFileReaderFactory::open(const char* filename,int flags) {
      SamFileReader* sfr = createNewSamFileReader();
      sfr->fp = HtsFile::open(filename, "rb");
		    if (sfr->fp== NULL) {
		       THROW_ERROR("Cannot open " << filename);
		    	}
		   
		    if (!sfr->fp->set_opt(CRAM_OPT_DECODE_MD, 0)) {
		        THROW_ERROR("Failed to set CRAM_OPT_DECODE_MD value.");
		   		}
			if(threadPool!=NULL) threadPool->bind(sfr->fp->fp);
			sfr->header= SamFileHeader::read(sfr->fp->fp,flags);
			if(sfr->header==NULL) {
			     THROW_ERROR("Cannot read header");
					goto error;
					}
			
			if(this->requires_index) {
				sfr->idx = HtsIndex::load(filename,0,0);
				if(sfr->idx==NULL) {
					THROW_ERROR("Cannot load index for " << filename);
					goto error;
					}
				}
			

			
			return sfr;
			  
			error:
			  if(sfr!=NULL) delete sfr;
			  return NULL;
			}
