/*
The MIT License (MIT)

Copyright (c) 2024 Pierre Lindenbaum

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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


