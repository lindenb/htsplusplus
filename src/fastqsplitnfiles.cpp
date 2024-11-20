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

#include "programs.hh"
#include "KSeq.hh"
#include "utils.hh"
using namespace std;

class SplitFile {
	public:
		std::string filename;
		gzFile out;
		unsigned long count;
		SplitFile(std::string fn,int level):filename(fn),out(NULL),count(0UL) {
			FILE* exists = fopen(filename.c_str(),"rb");
			if(exists!=NULL) {
				fclose(exists);
				THROW_ERROR("File already exists "<< filename << ".");
				}
		    char mode[5];
			sprintf(mode,"wb%d",level);
			out = gzopen(filename.c_str(),mode);
			if(out==NULL) {
				THROW_ERROR("Cannot open "<< filename << " for writing.(" << strerror(errno) << ")");
				}
			}
		void close() {
			DEBUG("closing " << filename << " N=" << count);
			gzclose(out);
			}
	};


int main_fastqsplitnfiles(int argc,char** argv) {
	FastqsplitnfilesArgs args;
	args.parse(argc,argv);
	PROGRAM_COMMON(args);
	
	if(StringUtils::isBlank(args.prefix)) {
		fprintf(stderr,"empty prefix.\n" );
		return EXIT_FAILURE;
		}
    if(args.nsplits<=0)  {
		cerr << "Bad value for nsplit :"<<  args.nsplits << endl;
		return EXIT_FAILURE;
        }
    if(args.compression_level<0 || args.compression_level>9)  {
		cerr << "Bad compression level:" << args.compression_level << endl;
		return EXIT_FAILURE;
        }
    vector<SplitFile*> splitFiles;
    splitFiles.reserve(args.nsplits);
   
    if(!(argc==optind || optind+1==argc))
		{
		fprintf(stderr,"Illegal number of arguments.\n");
		return EXIT_FAILURE;
		}

    for(int i=0;i< args.nsplits;i++) {
		char tmp[30];
		sprintf(tmp,".%05d.fastq.gz",(i+1));
		
		
		string filename(args.prefix);
		filename.append(tmp);
		SplitFile* sp = new SplitFile(filename,args.compression_level);
		
		splitFiles.push_back(sp);
		}
	

        long nReads = 0L;

		
		gzFile fp1 = (optind==argc?gzdopen(fileno(stdin), "r"):gzopen(argv[optind], "r"));
		if(fp1==NULL) {
			fprintf(stderr,"Cannot open %s.(%s)\n",(optind==argc?"<STDIN>":argv[optind]),strerror(errno));
			return EXIT_FAILURE;
			}
		KSeqReader* r = new KSeqReader(fp1);
		while (r->next())  {
			SplitFile* sp = splitFiles[nReads%splitFiles.size()];
			r->writeFastq(sp->out);		
			sp->count++;
			nReads++;			
			}
    	delete r;
		gzclose(fp1);

	    

     
    for(unsigned i=0;i< splitFiles.size();i++) {
		splitFiles[i]->close();
		delete splitFiles[i];
		}
	
	
    return EXIT_SUCCESS;
	}