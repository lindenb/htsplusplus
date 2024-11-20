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

#ifndef SAMFILE_WRITER_HH
#define SAMFILE_WRITER_HH
#include <cstring>
#include "SamFileReader.hh"

class SamFileWriter {
	public:
		std::unique_ptr<HtsFile> fp;
		bam_hdr_t *header;
		SamFileWriter() {
			}
	public:
		virtual void close() {
			fp->close();
			}
		virtual  ~SamFileWriter() {
			close();
			}
		void write_header(bam_hdr_t *header) {
			ASSERT_NOT_NULL(header);
			this->header = header;
			if(header==NULL || ::sam_hdr_write(fp->get(), this->header)!=0 ) {
				THROW_ERROR("Cannot write VCF header");
				}
			}

		void write1(bam1_t* rec) {
			int ret;
			ASSERT_NOT_NULL(rec);
		      	if((ret=::sam_write1(fp->get(), this->header, rec))<0) {
                		THROW_ERROR("IO error. Cannot write SAM record (error " << ret << ").");
                		}
			}
		void write2(SamRecord* rec) {
			ASSERT_NOT_NULL(rec);
			write1(rec->get());
			}
	};

class SamFileWriterFactory {
	private:
		int level;
		std::string output_format;
		std::string output_filename;
		HtsThreadPool* threadPool;
	public:
		SamFileWriterFactory():level(5),output_format("SAM"),threadPool(NULL) {
			}
		SamFileWriterFactory& compression(int level) {
			if(level>9) level=9;
			if(level<0) level=0;
			this->level = level;
			return *this;
			}
		SamFileWriterFactory& threads(HtsThreadPool* threadPool) {
			this->threadPool = threadPool;
			return *this;
			}
		SamFileWriterFactory& format(char* fmt) {
			if(fmt!=NULL) this->output_format.assign(fmt);
			return *this;
			}
		SamFileWriterFactory& output_file(char* filename) {
			if(filename==NULL) {
				output_filename.clear();
				}
			else
				{
				output_filename.assign(filename);
				}
			return *this;
			}
		std::unique_ptr<SamFileWriter> open(const char* filename) {
			char mode[12];
		    std::strcpy(mode, "wb");
			::sam_open_mode(&mode[1], filename, output_format.empty()?NULL:output_format.c_str());
			
			if (level >= 0) std::sprintf(std::strchr(mode, '\0'), "%d", level < 9? level : 9);
			std::unique_ptr<HtsFile> fp = HtsFile::open(filename, mode);
			
			if(threadPool!=NULL) threadPool->bind(fp->get());
			
			std::unique_ptr<SamFileWriter> w = std::unique_ptr<SamFileWriter>(new SamFileWriter());
			w->fp.swap(fp);
			return w;
			};
		std::unique_ptr<SamFileWriter> open() {
			return open(this->output_filename.empty()?"-":this->output_filename.c_str());
			}

	};


#endif
