#ifndef LOCATABLE_HH
#define LOCATABLE_HH
#include <htslib/hts.h>
#include <string>

template<typename POS_TYPE>
class StartEnd {
	public:
		virtual POS_TYPE getStart() const = 0;
		virtual POS_TYPE getEnd() const = 0;
	};

template<typename CONTIG_TYPE,typename POS_TYPE>
class Locatable: public StartEnd<POS_TYPE> {
	public:
		Locatable() {}
		virtual ~Locatable() {}
		virtual CONTIG_TYPE getContig() const = 0;
	};

template<typename CONTIG_TYPE,typename POS_TYPE>
class SimpleInterval: public Locatable<CONTIG_TYPE,POS_TYPE>
	{
	private:
		CONTIG_TYPE contig;
		POST_TYPE start;
		POST_TYPE end;
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
