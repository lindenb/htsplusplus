#include "UPileup.hh"

using namespace std;
using namespace htspp;

UPileup::UPileup() {
}

UPileup::~UPileup() {
}


void UPileup::scan(SamFileReader* reader) {
std::vector<SamFileReader*> v(1,reader);
MPileup::scan(v);
}

void UPileup::report(const std::vector<SamFileReader*>& readers,int32_t tid,hts_pos_t pos,const std::vector<int>& depth, const std::vector<bam_pileup1_t*>& mp) {
}
