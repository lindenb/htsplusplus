#include "SamSequenceRecord.hh"

using namespace htspp;
using namespace std;

SamSequenceRecord::SamSequenceRecord() {}
SamSequenceRecord::~SamSequenceRecord() {}
hts_pos_t SamSequenceRecord::start() const { return 1;}
hts_pos_t SamSequenceRecord::end() const { return length();}
