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
