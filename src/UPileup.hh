#ifndef U_PILEUP_H
#define U_PILEUP_H

#include "MPileup.hh"

namespace htspp {

class UPileup: public MPileup {
public:
   UPileup();
   virtual ~UPileup();
   virtual void scan(SamFileReader* reader);
   virtual void report(const std::vector<SamFileReader*>& readers,int32_t tid,hts_pos_t pos,const std::vector<int>& depth, const std::vector<bam_pileup1_t*>& mp) final;
   virtual void report(int tid,hts_pos_t pos,int depth, bam_pileup1_t* mp)=0;
};

}

#endif
