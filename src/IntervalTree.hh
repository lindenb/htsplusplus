#ifndef INTERVAL_TREE_HH
#define INTERVAL_TREE_HH

#include <map>
#include <vector>
#include <htslib/hts.h>
#include "CoordMath.hh"

namespace htspp {


template<typename T>
class IntervalTree {
private:
	struct start_end_object {
		int32_t start;
		int32_t end;
		T ptr;
		};
	typedef int bin_id;
	std::map<bin_id,std::vector<start_end_object>* > bin2data;
public:
	IntervalTree() {
		}
	
	virtual ~IntervalTree() {
		for(auto i= bin2data.begin(); i!=bin2data.end();++i) {
			auto vec = i->second;
			delete vec;
			}
		}

    
	
	virtual void put(int32_t start,int32_t end,T ptr) {
		int bin = ::hts_reg2bin(start,end, 14, 5);
		auto i = bin2data.find(bin);
		start_end_object pair;
		pair.start = start;
		pair.end = end;
		pair.ptr = ptr;
		std::vector<start_end_object>* vec = NULL;
		if(i== bin2data.end()) {
			vec = new std::vector<start_end_object>;
			bin2data.insert(make_pair(bin,vec));
			}
		else
			{
			vec = i->second;
			}
		vec->push_back(pair);
		}
	


	virtual void find_overlapping(int32_t start,int32_t end,std::vector<T>& collector) const{
	    int bin = ::hts_reg2bin(start,end, 14, 5);
		auto i = bin2data.find(bin);
		if(i != bin2data.end())  {
			auto vec = i->second;
			for(auto j=vec->begin(); j!=vec->end(); ++j) {
				if(CoordMath::overlaps(j->start,j->end,start,end)) {
					collector.push_back(j->ptr);
					}
				}
			}
		}
    virtual std::vector<T> find_overlapping(int32_t start,int32_t end) const {
        std::vector<T> collector;
        this->find_overlapping(start,end,collector);
        return collector;
        }
	};


}

#endif
