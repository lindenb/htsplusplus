#ifndef MPILEUP_H
#define MPILEUP_H

#include <vector>
#include <htslib/hts.h>
#include <htslib/sam.h>
#include "SamFileReader.hh"

namespace htspp {
/*
class MPileup {
private:
public:
   int max_depth;
   MPileup();
   virtual ~MPileup();
   virtual void scan(std::vector<SamFileReader*> readers);
   virtual int read(bam1_t *b);
   virtual void report(const std::vector<SamFileReader*>& readers,int32_t tid,hts_pos_t pos,const std::vector<int>& depth, const std::vector<bam_pileup1_t*>& mp)=0;
};

}*/



template<typename T>
class MPileup {
protected:
	bam_mplp_t mplp;
	std::vector<std::pair<MPileup*,T> > data;
public:
	MPileup(unsigned int nbams) {
	  extern bam_plp_auto_f _read_bam,;
	  this->mplp = ::bam_mplp_init(nbams , _read_bam, (void**)this); // initialization
	}
	virtual ~MPileup() {
		::bam_mplp_destroy(mplp);
	}
}

static int _read_bam(void *data, bam1_t *b) {
    MPileup* owner=(MPileup*)data;
  return owner->read(b);
    }

MPileup::MPileup():max_depth(INT_MAX) {
}


void MPileup::scan(vector<SamFileReader*> readers) {
  int ret;
  bam_mplp_set_maxcnt(mplp, max_depth);  // set maximum coverage depth
  int tid;
  hts_pos_t pos;

  std::vector<int> n_plp(readers.size(),0);
  std::vector<bam_pileup1_t*> plp(readers.size(),NULL);
 while ((ret=bam_mplp64_auto(mplp, &tid, &pos, const_cast<int*>(n_plp.data()), (const bam_pileup1_t**)plp.data() )) > 0) { // come to the next covered position
  report(readers,tid,pos,n_plp,plp);
  }
}

MPileup::~MPileup() {
}


#endif
