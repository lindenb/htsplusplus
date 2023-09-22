
#include <vector>
#include <unordered_map>
#include "debug.hh"
#include "SamSequenceDictionary.hh"

using namespace std;


class DefaultSamSequenceDictionary : public SamSequenceDictionary{
	protected:
        class SamSequenceRecord {
            public:
                std::string name;
                hts_pos_t length;
            };
        std::vector<SamSequenceRecord> ssrs;
        std::unordered_map<std::string,unsigned int> name2tid;
        unsigned long _len;
    public:
        DefaultSamSequenceDictionary():_len(0UL) {
            }
        virtual ~DefaultSamSequenceDictionary() {
            }
        void put(std::string contig,hts_pos_t ctglentgh) {
            if(name2tid.find(contig)!=name2tid.end()) THROW_ERROR("duplicate name "<< contig);
            SamSequenceRecord ssr;
            ssr.name = contig;
            ssr.length = ctglentgh;
            name2tid.insert(make_pair(ssr.name,ssrs.size()));
            ssrs.push_back(ssr);
            _len += ctglentgh;
            }
		virtual unsigned int nseq() const {
            return ssrs.size();
            }
		virtual const char* name(unsigned int i) const {
            return ssrs[i].name.c_str();
            }
		virtual hts_pos_t length(unsigned int i) const {
            return ssrs[i].length;
            }
      virtual unsigned int tid(const char* ctg) const {
            auto r=name2tid.find(ctg);
            if(r==name2tid.end()) return SamSequenceDictionary::npos;
            return r->second;
            }
        virtual unsigned long genome_length() const  {
            return _len;
            }

    };



std::unique_ptr<SamSequenceDictionary> SamSequenceDictionary::of(bcf_hdr_t *header) {
	ASSERT_NOT_NULL(header);
    DefaultSamSequenceDictionary* dict = new DefaultSamSequenceDictionary;
    THROW_ERROR("todo");
	return std::unique_ptr<SamSequenceDictionary>(dict);
	}



std::unique_ptr<SamSequenceDictionary> SamSequenceDictionary::of(faidx_t *fai) {
	ASSERT_NOT_NULL(fai);
	 DefaultSamSequenceDictionary* dict = new DefaultSamSequenceDictionary;
	for(int i=0;i< ::faidx_nseq(fai);i++) {
		dict->put(  ::faidx_iseq(fai,i),  ::faidx_seq_len64(fai, ::faidx_iseq(fai,i)) );
		}
	return std::unique_ptr<SamSequenceDictionary>(dict);
	}

