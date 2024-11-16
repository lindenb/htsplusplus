
#include <vector>
#include <unordered_map>
#include "debug.hh"
#include "SamSequenceDictionary.hh"

using namespace std;
using namespace htspp;

class DefaultSamSequenceDictionary : public SamSequenceDictionary{
	protected:
        class SamSequenceRecord {
            public:
            	  int32_t tid;
                std::string name;
                hts_pos_t length;
            };
        std::vector<SamSequenceRecord*> ssrs;
        std::unordered_map<std::string,SamSequenceRecord*> name2ssr;
        unsigned long _len;
    public:
        DefaultSamSequenceDictionary():_len(0UL) {
            }
        virtual ~DefaultSamSequenceDictionary() {
        	for(size_t i=0;i< ssrs.size();i++) {
        			delete 	ssrs[i];
        			}
            }
        void put(std::string contig,hts_pos_t ctglentgh) {
            if(name2ssr.find(contig)!=name2ssr.end()) THROW_ERROR("duplicate name "<< contig);
            SamSequenceRecord* ssr=new SamSequenceRecord;
            ssr->tid =(int32_t)ssrs.size();
            ssr->name = contig;
            ssr->length = ctglentgh;
            name2ssr.insert(make_pair(ssr->name,ssr));
            ssrs.push_back(ssr);
            _len += ctglentgh;
            }
		virtual int32_t nseq() const {
            return ssrs.size();
            }
		virtual const char* name(int32_t i) const {
            return ssrs[i]->name.c_str();
            }
		virtual hts_pos_t length(int32_t i) const {
            return ssrs[i]->length;
            }
      virtual int32_t tid(const char* ctg) const {
            auto r=name2ssr.find(ctg);
            if(r==name2ssr.end()) return -1;
            return r->second->tid;
            }
        virtual long genome_length() const  {
            return _len;
            }

    };

SamSequenceDictionary::SamSequenceDictionary() {
}

SamSequenceDictionary::~SamSequenceDictionary() {
}

bool SamSequenceDictionary::equals(const SamSequenceDictionary& cp) const {
			if(&cp==this) return true;
			if(cp.nseq()!=nseq()) return false;
			for(int32_t i=0;i< nseq();++i) {
				if(length(i)!=cp.length(i)) return false;
				if(strcmp(name(i),cp.name(i))!=0) return false;
				}
			return true;
			}

bool SamSequenceDictionary::operator==(const SamSequenceDictionary& cp) const {
	return this->equals(cp);
	}

std::unique_ptr<SamSequenceDictionary> SamSequenceDictionary::of_sam_header(sam_hdr_t *header) {
	ASSERT_NOT_NULL(header);
  DefaultSamSequenceDictionary* dict(new DefaultSamSequenceDictionary);
  for(int i=0;i< header->n_targets;i++) {
  	dict->put(header->target_name[i],header->target_len[i]);
  	}
   std::unique_ptr<DefaultSamSequenceDictionary> p(dict);
	return p;
	}



std::unique_ptr<SamSequenceDictionary> SamSequenceDictionary::of_bcf_header(bcf_hdr_t *header) {
	ASSERT_NOT_NULL(header);
    DefaultSamSequenceDictionary* dict = new DefaultSamSequenceDictionary;
    THROW_ERROR("todo");
	return std::unique_ptr<SamSequenceDictionary>(dict);
	}


std::unique_ptr<SamSequenceDictionary> SamSequenceDictionary::of_faidx(faidx_t *fai) {
	ASSERT_NOT_NULL(fai);
	 DefaultSamSequenceDictionary* dict = new DefaultSamSequenceDictionary;
	for(int i=0;i< ::faidx_nseq(fai);i++) {
		dict->put(  ::faidx_iseq(fai,i),  ::faidx_seq_len64(fai, ::faidx_iseq(fai,i)) );
		}
	return std::unique_ptr<SamSequenceDictionary>(dict);
	}

