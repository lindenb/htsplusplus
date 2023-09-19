#ifndef LOCATABLE_HH
#define LOCATABLE_HH
#include <htslib/hts.h>


class Locatable {
	public:
		Locatable() {}
		virtual ~Locatable() {}
		virtual const char* contig()=0;
		virtual hts_pos_t start()=0;
		virtual hts_pos_t end() =0;
	};


#endif
