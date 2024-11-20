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

#include "RegIdx.hh"


#include "SamFilterApp.hh"
#include "programs.hh"

using namespace std;

/*
template <class T>
class ProgramBamFilter : public T {
public:
	ProgramBamFilter() {}
	std::unique_ptr<SamFileWriter> open_sam_file_writer() {
			SamFileWriterFactory swf;
			swf.compression(this->compression);
			swf.format(this->bam_output_format);
			return swf.open(this->output_filename);
			}
	
};*/


int main_samviewwithmate(int argc, char** argv) {
	SamviewwithmateArgs args;
	if(!args.parse(argc,argv)) return EXIT_FAILURE;
	PROGRAM_COMMON(args);
	DEBUG(args.nthreads);
	std::unique_ptr<HtsThreadPool> thread_pool  =  HtsThreadPool::create(args.nthreads);
	SamFileReaderFactory srf;
	const char* input = args.oneFileOrNull();
	std::unique_ptr<SamFileReader> r=srf.
		threads(thread_pool.get()).
		open(input==NULL?"-":input);
	if(r->header->is_coordinate_sorted()) {
		THROW_ERROR("input should not be coordinate sorted");
		}
	SamFileWriterFactory swf;
	std::unique_ptr<RegIdx> regidx = RegIdx::load_file(args.bed_file);

	std::unique_ptr<SamFileWriter> w=swf.
		threads(thread_pool.get()).
		compression(args.compression_level).
		format(args.bam_output_format).
		open("-");

	w->write_header(r->header->get());
	std::vector<SamRecord*> pool;
	std::vector<SamRecord*> buffer;
	bool in_bed=false;
	for(;;) {
		while(buffer.size() >= pool.size()) {
			pool.push_back(new SamRecord);
			}

		SamRecord* rec = pool[buffer.size()];
		if(!r->read2(rec)) {
			rec=NULL;
			}
		if(rec==NULL ||  (!buffer.empty() && strcmp(buffer[0]->read_name(),rec->read_name())!=0)) {
			if(args.inverse) in_bed=!in_bed;
			if(in_bed) {
				for(unsigned int i=0;i< buffer.size();i++) {
					w->write2(buffer[i]);
					}
				}
			buffer.clear();
			in_bed=false;
			if(rec==NULL) break;
			}
		if(!in_bed && !rec->is_unmapped() ) {
			const char* contig = r->header->tid2name(rec->tid());
			in_bed = regidx->overlaps(contig,rec->start(),rec->end());
			}
		buffer.push_back(rec);
		}

	for(unsigned int i=0;i< pool.size();i++) {
		delete pool[i];
		}
	w->close();
	return 0;
	}
