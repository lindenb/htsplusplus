#ifndef CONTIG_CONVERTER_HH
#define CONTIG_CONVERTER_HH
#include <unsorted_map>
#include <set>
#include <string>


class ContigConverter {
	private:
		std::unsorted_map<std:string,std:string> ctg2ctg;
	public:
		ContigConverter(std::set<std::string>& contigs) {
			auto r= contigs.begin();
			while(r!=contigs.end()) {
				std::string ctg = *r;
				ctg2ctg.insert(make_pair(ctg,ctg));
				if(ctg.length()>3 && ctg.compare(0,3,"chr")==0) {
					std::string ctg2(ctg);
					ctg2.erase(0,3UL);
					if(contigs.find(ctg2)==contigs.end()) {
						ctg2ctg.insert(make_pair(ctg2,ctg));
						}
					}
				else
					{
					std::string ctg2(ctg);
					ctg2.insert(0,3,"chr");
					if(contigs.find(ctg2)==contigs.end()) {
						ctg2ctg.insert(make_pair(ctg2,ctg));
						}
					}

				++r;
				}
			}
		const char* apply(const char* chromosome)  {
			if(chromosome==NULL) return NULL;
			std::string ctg(chromosome);
			auto r = ctg2ctg.find(ctg);
			if(r != ctg2ctg.end()) return r->second.c_str();
			return NULL;
			}

	};


#endif

