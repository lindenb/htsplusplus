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

#include <cassert>
#include <sstream>
#include "SamRecord.hh"

using namespace htspp;
using namespace std;

SamRecord& SamRecord::operator=(const SamRecord& cp) {
if(this!=&cp) {
    ::bam_destroy1(this->b);
    this->b=::bam_dup1(cp.b);
    assert(b!=NULL);
    this->header = cp.header;
    }
return *this;
}


		bam1_t* SamRecord::get() {return b;}
		bam1_t* SamRecord::get() const {return b;}


		SamRecord::SamRecord():b(::bam_init1()),header(NULL) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}

			}
		SamRecord::SamRecord(SamFileHeader* header):b(::bam_init1()),header(header) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}

			}
		SamRecord::SamRecord(const SamRecord& cp):b(::bam_dup1(cp.b)),header(cp.header) {
			if(b==NULL) {
				THROW_ERROR("Out of memory.");
				}
			}

	 SamRecord::~SamRecord() {
			if(b!=NULL) ::bam_destroy1(b);
			}

       const char* SamRecord::contig() const {return header->tid2name(tid());}
       hts_pos_t SamRecord::start() const { return AbstractSamRecord::start();}
       hts_pos_t SamRecord::end() const { return AbstractSamRecord::end();}


		std::unique_ptr<SamRecord> SamRecord::clone() {
			return std::unique_ptr<SamRecord>(new SamRecord(*this));
			}


		std::ostream& SamRecord::print(std::ostream& out) const {
			KString str;
			if (sam_format1(header->get(), get(), str.get()) < 0);
			return str.print(out);
			}
		std::string SamRecord::to_string() const {
			ostringstream os;
			print(os);
			return os.str();
			}
