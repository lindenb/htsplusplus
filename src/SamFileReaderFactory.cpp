/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include "SamFileReaderFactory.hh"

using namespace std;
using namespace htspp;

SamFileReader* SamFileReaderFactory::createNewSamFileReader() {
	    return new SamFileReader;
	    }
SamFileReaderFactory::SamFileReaderFactory():threadPool(NULL) {
			}

SamFileReaderFactory& SamFileReaderFactory::threads(HtsThreadPool* threadPool) {
			this->threadPool = threadPool;
			return *this;
			}



SamFileReaderFactory& SamFileReaderFactory::reference(const char* fasta) {
		this->_reference.assign(fasta);
		return *this;
		}

		
SamFileReaderFactory::~SamFileReaderFactory() {
  }


unique_ptr<SamFileReader> SamFileReaderFactory::open() {
	return open("-");
	}

unique_ptr<SamFileReader> SamFileReaderFactory::open(const char* filename) {
	return open(filename,false);
	}
	

unique_ptr<SamFileReader> SamFileReaderFactory::open(const char* filename,bool require_index) {
	return open(filename,NULL,require_index);
	}


std::unique_ptr<SamFileReader> SamFileReaderFactory::open(const char* filename,const char* index_or_NULL,bool require_index) {
      unique_ptr<SamFileReader> sfr(createNewSamFileReader());
      sfr->fp = HtsFile::open(filename, "rb");
	    if (!(sfr->fp)) {
	       THROW_ERROR("Cannot open " << filename);
	    	}
		   
	    if (sfr->fp->set_opt(CRAM_OPT_DECODE_MD, 0)) {
	        THROW_ERROR("Failed to set CRAM_OPT_DECODE_MD value.");
	   		}
			if(threadPool!=NULL) threadPool->bind(sfr->fp->get());
			sfr->header = SamFileHeader::read(sfr->fp->get(),0);
			if(!sfr->header) {
			     THROW_ERROR("Cannot read header");
					goto error;
					}
			
			if(require_index) {
				hts_idx_t* i = ::sam_index_load2( sfr->fp->get(), filename,index_or_NULL);
 				if(i==NULL) THROW_ERROR("cannot load CRAM/BAM index for " << filename);
				sfr->idx =  HtsIndex::of(i);
				}
				
	    if(!_reference.empty()) {
          if (hts_set_fai_filename(sfr->fp->get(),_reference.c_str()) != 0) {
						THROW_ERROR("Cannot set REF for " << filename);
              }
          }
			
			  return sfr;

			error:
			unique_ptr<SamFileReader> p2;
			  return p2;
			}
