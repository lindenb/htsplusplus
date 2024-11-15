#include <cassert>
#include "SamRecord.hh"

using namespace htspp;


SamRecord& SamRecord::operator=(const SamRecord& cp) {
if(this!=&cp) {
    ::bam_destroy1(this->b);
    this->b=::bam_dup1(cp.b);
    assert(b!=NULL);
    this->header = cp.header;
    }
return *this;
}
