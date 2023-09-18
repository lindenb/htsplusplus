#include <htslib/vcf.h>
#include <vector>
#include "debug.hh"
#include "SamSequenceDictionary.hh"

using namespace std;
/*
class BcfSamSequenceRecord : public SamSequenceRecord {
	private:
		bcf_hdr_t *header;
		int rid;
	public:
		BcfSamSequenceRecord( bcf_hdr_t *header, int rid):header(header),rid(rid)) {}
		virtual ~BcfSamSequenceRecord() {}
		virtual const char* name() {
			return ::bcf_hdr_id2name(header,rid);
			}
	};
		

		class BcfSamSequenceDictionary : public SamSequenceDictionary {
			private:
				std::vector<BcfSamSequenceRecord*> array;
			public:
				BcfSamSequenceDictionary(  bcf_hdr_t *header) {
					ASSERT_NOT_NULL(header->dict[BCF_DT_CTG]);
					for(int i=0;i< header->n[BCF_DT_CTG];i++) {
						array.push_back(int BcfSamSequenceRecord(header,i));
						}
					vdict_t* d = (vdict_t*)(header->dict[BCF_DT_CTG]);
					unsigned int n= kh_size(d);
					khint_t k;
					for (k=kh_begin(d); k<kh_end(d); k++) {
						
						}
					}
				virtual ~BcfSamSequenceDictionary() {
					for(unsigned int i=0;i< array.size();i++) delete array[i];
					}
				virtual SamSequenceDictionary::size_type size() { return array.size();};
				virtual SamSequenceRecord* get(unsigned int idx) { return array.at(idx);};
			};
*/
std::unique_ptr<SamSequenceDictionary> SamSequenceDictionary::of(bcf_hdr_t *header) {
	ASSERT_NOT_NULL(header);
	//return std::unique_ptr<SamSequenceDictionary>(new BcfSamSequenceDictionary(header));
	THROW_ERROR("not implemented");
	}

