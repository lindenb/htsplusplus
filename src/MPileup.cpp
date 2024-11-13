#include "MPileup.hh"

static int _read_bam(void *data, bam1_t *b) {
	MPileup* owner=(MPileup*)data;
  return owner->read(b);
	}

MPileup::MPileup():mplp(NULL) {
}

MPileup::init() {
  mplp = bam_mplp_init(n_bam_files, _read_bam, (void**)this); // initialization
    if (max_depth > 0)
        bam_mplp_set_maxcnt(mplp, max_depth);  // set maximum coverage depth
    else if (!max_depth)
        bam_mplp_set_maxcnt(mplp, INT_MAX);
	}

MPileup::~MPileup() {
 dispose();
 
void MPileup::dispose() {
if (mplp!=NULL) bam_mplp_destroy(mplp);
mplp=NULL;
}

void MPileup::scan() {
 int** n_plp = (int*) calloc(n_bam_files, sizeof(int*)); // n_plp[i] is the number of covering reads from the i-th BAM
 const bam_pileup1_t** plp = (const bam_pileup1_t**) calloc(n_bam_files, sizeof(bam_pileup1_t*)); // plp[i] points to the array of covering reads (internal in mplp)
 while ((ret=bam_mplp_auto(mplp, &tid, &pos, n_plp, plp)) > 0) { // come to the next covered position
  report();
  }
if (n_plp) free(n_plp);
if (plp) free(plp);
}
