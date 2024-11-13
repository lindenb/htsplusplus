#ifndef MPILEUP_H
#define MPILEUP_H

#include <htslib/hts.h>
#include <htslib/sam.h>

class MPileup {
private:
	bam_mplp_t mplp;
public:
   MPileup();
   virtual void dispose();
	 virtual ~MPileup();
	 virtual int read( bam1_t *b);
};

#endif
