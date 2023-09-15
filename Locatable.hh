#ifndef LOCATABLE_HH
#define LOCATABLE_HH
#include <htslib/hts.h>
#include <string>

class Locatable {
	public:
		Locatable() {}
		virtual ~Locatable() {}
		virtual const char* getContig() const = 0;
		virtual hts_pos_t getStart() const = 0;
		virtual hts_pos_t getEnd() const = 0;
	};

class SimpleInterval: public Locatable
	{
	private:
		std::string contig;
		hts_pos_t start;
		hts_pos_t end;
	public:
		SimpleInterval(std::string contig,  hts_pos_t start,  hts_pos_t end):contig(contig),start(start),end(end) {
			}
		SimpleInterval(const Locatable& cp):contig(cp.getContig()),start(cp.getStart()),end(cp.getEnd())
			}
		virtual ~SimpleInterval() {
			}
		virtual const char* getContig() const { return contig.c_str();}
                virtual hts_pos_t getStart() const { return start;};
                virtual hts_pos_t getEnd() const { return end;}

	};

#endif
