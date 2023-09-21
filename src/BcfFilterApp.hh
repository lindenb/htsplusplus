#ifndef BCF_FILTER_APP_HH
#define BCF_FILTER_APP_HH

#include "BcfHeader.hh"
#include "BcfReader.hh"
#include "BcfWriter.hh"
#include "debug.hh"


template<class T>
class BcfFilterApp: public T
	{
	public:
		BcfFilterApp() {
			}
		virtual ~BcfFilterApp() {
			}
		virtual int beforeBcf() {
			return 0;
			}
		virtual void afterBcf() {
			}

        virtual std::unique_ptr<BcfHeader> create_output_header(BcfHeader h) {
            return h->clone();            
            }

        virtual bool accept(BcfRecordHeader* rec) {
            return true;            
            }
        
        virtual int apply(BcfReader* in, BcfWriter* out) {
            BcfRecordHeader bcf;
            while(in->read2(&bcf)) {
                  if(!accept(&bcf)) continue;
                  out->write1(bcf.get());
                  }
            return 0;       
            }

		virtual int apply(const char* filein, const char* fileout) {
			if(beforeBcf()!=0) {
				WARNING("initialization failed");
				return EXIT_FAILURE;
				}
	        std::unique_ptr<BcfReader> in = BcfReader::open(filein==NULL ?"-":argv[optind]);
            BcfWriterFactory writer_factory;
            std::unique_ptr<BcfWriter> out = writer_factory.format(this->bcf_output_format).open(fileout);
            std::unique_ptr<BcfHeader> out_header = create_output_header(in->header.get());
            out->write_header(in->header->get());
            int ret = apply(in,out);
			afterBcf();
			return ret;
			}

	};


#endif

