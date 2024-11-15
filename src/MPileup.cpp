#include <memory>
#include <climits>
#include "utils.hh"
#include "MPileup.hh"

using namespace std;
using namespace htspp;

static int _read_bam(void *data, bam1_t *b) {
	MPileup* owner=(MPileup*)data;
  return owner->read(b);
	}

MPileup::MPileup():max_depth(INT_MAX) {
}


void MPileup::scan(vector<SamFileReader*> readers) {
  int ret;
  bam_mplp_t mplp = bam_mplp_init(readers.size() , _read_bam, (void**)this); // initialization
  bam_mplp_set_maxcnt(mplp, max_depth);  // set maximum coverage depth
  int tid;
  hts_pos_t pos;

  std::vector<int> n_plp(readers.size(),0);
  std::vector<bam_pileup1_t*> plp(readers.size(),NULL);
 while ((ret=bam_mplp64_auto(mplp, &tid, &pos, const_cast<int*>(n_plp.data()), (const bam_pileup1_t**)plp.data() )) > 0) { // come to the next covered position
  report(readers,tid,pos,n_plp,plp);
  }
bam_mplp_destroy(mplp);
}

MPileup::~MPileup() {
}


