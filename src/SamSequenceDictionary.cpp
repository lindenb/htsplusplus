
#include <vector>
#include <unordered_map>
#include "debug.hh"
#include "SamSequenceDictionary.hh"
#include "GenomicLocParser.hh"
using namespace std;
using namespace htspp;

class DefaultSamSequenceDictionary : public SamSequenceDictionary{
	protected:
        class SamSequenceRecordImpl : public SamSequenceRecord {
            public:
            	  int32_t _tid;
                std::string _name;
                hts_pos_t _length;
                SamSequenceRecordImpl():SamSequenceRecord() {}
                virtual ~SamSequenceRecordImpl() {}
                virtual const char* contig() const { return this->name();}
                virtual const char* name() const { return this->_name.c_str();}
                virtual hts_pos_t start() const { return 1;}
                virtual hts_pos_t end() const { return length();}
                virtual hts_pos_t length() const { return _length;}
                virtual int32_t tid() const { return _tid;}
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
            SamSequenceRecordImpl* ssr=new SamSequenceRecordImpl;
            ssr->_tid =(int32_t)ssrs.size();
            ssr->_name = contig;
            ssr->_length = ctglentgh;
            name2ssr.insert(make_pair(ssr->_name,ssr));
            ssrs.push_back(ssr);
            _len += ctglentgh;
            }
    virtual const SamSequenceRecord* at(int32_t i) const {
    			return ssrs.at(i);
    			}
    virtual const SamSequenceRecord* getSequence(const char* ctg) const {
    			  auto r=name2ssr.find(ctg);
            if(r==name2ssr.end()) return NULL;
            return r->second;
    			}
		virtual int32_t nseq() const {
            return ssrs.size();
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
				if(at(i)->length()!=cp.at(i)->length()) return false;
				if(strcmp(at(i)->name(),cp.at(i)->name())!=0) return false;
				}
			return true;
			}

bool SamSequenceDictionary::operator==(const SamSequenceDictionary& cp) const {
	return this->equals(cp);
	}


std::unique_ptr<std::string> SamSequenceDictionary::resolve_contig(const char* s) const {
	std::unique_ptr<std::string> p;
	const SamSequenceRecord* ssr = getSequence(s);
	if(ssr!=NULL) {
		p.reset(new std::string(ssr->contig()));
		return p;
		}
	else if(strncmp(s,"chr",3)==0) {
		ssr = getSequence(&s[3]);
		}
	else
		{
		std::string s2("chr");
		s2.append(s);
		ssr = getSequence(s2.c_str());
		}
	if(ssr!=NULL) {
			p.reset(new std::string(ssr->contig()));
			}
	return p;
	}

std::unique_ptr<Locatable> SamSequenceDictionary::parseInterval(const char* s) const {
	GenomicLocParser glp(this);
	glp.resolve_contigs(true);
	glp.trim_positions(true);
	glp.enable_contig_only(true);
	return glp.parse(s);
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

