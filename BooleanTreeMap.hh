#ifndef INTERVAL_TREEMAP_HH
#define INTERVAL_TREEMAP_HH

#include <string>
#include <map>
#include <vector>
#include <htslib/hts.h>

#define MAX_BIN 37450 // =(8^6-1)/7+1

class BooleanTreeMap {

private:
	uint16_t* list;
	struct StartEnd{int32_t start,end;};
	std::map<std::string, std::map<int32_t,std::vector<StartEnd>* >* > contig2opcode;
	
	int32_t ti_reg2bin(int32_t beg,int32_t end) {
	--end;
	if (beg>>14 == end>>14) return 4681 + (beg>>14);
	if (beg>>17 == end>>17) return  585 + (beg>>17);
	if (beg>>20 == end>>20) return   73 + (beg>>20);
	if (beg>>23 == end>>23) return    9 + (beg>>23);
	if (beg>>26 == end>>26) return    1 + (beg>>26);
	return 0;
		}

	int reg2bins(uint32_t beg, uint32_t end)
	{
	int i = 0, k;
	if (beg >= end) return 0;
	if (end >= 1u<<29) end = 1u<<29;
	--end;
	list[i++] = 0;
	for (k =    1 + (beg>>26); k <=    1 + (end>>26); ++k) list[i++] = k;
	for (k =    9 + (beg>>23); k <=    9 + (end>>23); ++k) list[i++] = k;
	for (k =   73 + (beg>>20); k <=   73 + (end>>20); ++k) list[i++] = k;
	for (k =  585 + (beg>>17); k <=  585 + (end>>17); ++k) list[i++] = k;
	for (k = 4681 + (beg>>14); k <= 4681 + (end>>14); ++k) list[i++] = k;
	return i;
	}

public:
	BooleanTreeMap() {
		this->list = new uint16_t[MAX_BIN];
		}
	~BooleanTreeMap() {
		delete[] this->list;
		}
	void insert(const char* s,int32_t b,int32_t e) {
		std::map<int32_t,std::vector<StartEnd>* >* m2 = 0;
		std::map<std::string, std::map<int32_t,std::vector<StartEnd>* >* >::iterator r1 = this->contig2opcode.find(s);
		if(r1==  this->contig2opcode.end()) {
			m2 = new std::map<int32_t,std::vector<StartEnd>* >;
			contig2opcode.insert(make_pair(s,m2));
			}
		else	{
			m2= r1->second;
			}		
		int op = ti_reg2bin(b,e);
		std::vector<StartEnd>* vec = 0;
		std::map<int32_t,std::vector<StartEnd>* >::iterator r2 = m2->find(op);
		if(r2 == m2->end()) {
			vec = new std::vector<StartEnd>;
			m2->insert(make_pair(op,vec));
			}
		else	{
			vec = r2->second;
			}
		StartEnd se={b,e};
		vec->push_back(se);
		}
	bool contains(const char* s,int32_t b,int32_t e) {
                std::map<std::string, std::map<int32_t,std::vector<StartEnd>* >* >::iterator r1 = this->contig2opcode.find(s);
                if(r1==  this->contig2opcode.end()) return false;
		std::map<int32_t,std::vector<StartEnd>* >* m2= r1->second;
		
		int  i,n =reg2bins(b,e);
		for(i=0;i< n;i++) {
			int op =  this->list[i];
			std::map<int32_t,std::vector<StartEnd>* >::iterator r2 = m2->find(op);
			if(r2==m2->end()) continue;
			std::vector<StartEnd>* vec = r2->second;
			for(std::vector<StartEnd>::iterator r3 = vec->begin();r3!=vec->end();r3++) {
				if(r3->end < b ) continue;
				if(r3->start > e ) continue;
				return true;
				}
			}
		return false;
		}
	void readBed(const char* fname) {
		}
	};

#undef MAX_BIN

#endif

