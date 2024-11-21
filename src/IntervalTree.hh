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


 int reg2bin(int beg, int end) const
  {
    --end;
    if (beg>>14 == end>>14) return ((1<<15)-1)/7 + (beg>>14);
    if (beg>>17 == end>>17) return ((1<<12)-1)/7 + (beg>>17);
    if (beg>>20 == end>>20) return ((1<<9)-1)/7 + (beg>>20);
    if (beg>>23 == end>>23) return ((1<<6)-1)/7 + (beg>>23);
    if (beg>>26 == end>>26) return ((1<<3)-1)/7 + (beg>>26);
    return 0;
  }
  void reg2bins(int beg, int end, std::vector<uint16_t>& lst) const
  {
    int k;
    --end;
    lst.push_back(0);
    for (k = 1 + (beg>>26); k <= 1 + (end>>26); ++k) lst.push_back(k);
    for (k = 9 + (beg>>23); k <= 9 + (end>>23); ++k) lst.push_back(k);
    for (k = 73 + (beg>>20); k <= 73 + (end>>20); ++k) lst.push_back(k);
    for (k = 585 + (beg>>17); k <= 585 + (end>>17); ++k) lst.push_back(k);
    for (k = 4681 + (beg>>14); k <= 4681 + (end>>14); ++k) lst.push_back(k);
  }
	
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
		int bin = reg2bin(start,end);
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
	    std::vector<uint16_t> lst;
	    reg2bins(start,end,lst);
	    for(auto iter=lst.begin();iter!=lst.end();++iter) {
	    int bin = *iter;
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
		}
    virtual std::vector<T> find_overlapping(int32_t start,int32_t end) const {
        std::vector<T> collector;
        this->find_overlapping(start,end,collector);
        return collector;
        }
	};


}

#endif
