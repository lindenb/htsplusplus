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
#ifndef INTERVAL_TREE_MAP_HH
#define INTERVAL_TREE_MAP_HH

#include <string>
#include "IntervalTree.hh"

namespace htspp {


template<typename T>
class IntervalTreeMap {
private:
	std::map<std::string,IntervalTree<T>* > contig2tree;
public:
	IntervalTreeMap() {
		}
	
	virtual ~IntervalTreeMap() {
		for(auto i=contig2tree.begin();i!=contig2tree.end();++i) {
			delete i->second;
			}
		}
	
	
	virtual void put(Locatable* loc,T obj) {
		put(loc->contig(),loc->start(),loc->end(), obj);
		}
	
	virtual void put(const char* contig,hts_pos_t start, hts_pos_t end,T obj) {
		auto i = contig2tree.find(contig);
		IntervalTree<T>* tree = NULL;
		if(i== contig2tree.end()) {
			tree = new IntervalTree<T>;
			contig2tree.insert(std::make_pair(contig,tree));
			}
		else
			{
			tree = i->second;
			}
		tree->put(start,end,obj);
		}

	virtual void find_overlapping(Locatable* loc,std::vector<T>& collector) {
		auto i = contig2tree.find(loc->contig());
		if(i != contig2tree.end())  {
			i->second->find_overlapping(loc->start()-1,loc->end(),collector);
			}
		}

	};


}

#endif
