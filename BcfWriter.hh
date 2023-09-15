#ifndef BCF_WRITER_HH
#define BCF_WRITER_HH

#include "HtsFile.hh"
#include "BcfHeader.hh"
#include "BcfRecord.hh"

class BcfWriter {
	public:
		std::unique_ptr<HtsFile> fp;
		bcf_hdr_t *header;
		BcfWriter() {
			}
	public:
		virtual void close() {
			fp->close();
			}
		virtual  ~BcfWriter() {
			close();
			}
		void write_header(bcf_hdr_t *header) {
			this->header = header;
			if(header==NULL || ::bcf_hdr_write(fp->get(), this->header)!=0 ) {
				THROW_ERROR("Cannot write VCF header");
				}
			}

		void write1(bcf1_t* rec) {
		      	if(::bcf_write1(fp->get(), this->header, rec)!=0) {
                		THROW_ERROR("IO error. Cannot write VCF record.");
                		}
			}
		void write2(BcfRecord* rec) {
			write1(rec->get());
			}
	};

class BcfWriterFactory {
	private:
		int level;
		char output_format;
	public:
		BcfWriterFactory():level(5),output_format('v') {
			}
		BcfWriterFactory& compression(int level) {
			if(level>9) level=9;
			if(level<0) level=0;
			this->level = level;
			return *this;
			}
		BcfWriterFactory& format(char fmt) {
			this->output_format = fmt;
			return *this;
			}
		std::unique_ptr<BcfWriter> open(const char* filename) {
			char format[4];
			format[0] = 'w';
			format[1] = output_format;
			format[2] = '0'+(level);
			format[3] = 0;
			std::unique_ptr<HtsFile> fp = HtsFile::open(filename, format);
			std::unique_ptr<BcfWriter> w = std::unique_ptr<BcfWriter>(new BcfWriter());
			w->fp.swap(fp);
			return w;
			};
	};


#endif
