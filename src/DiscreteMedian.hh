#ifndef DISCRETE_MEDIAN_HH
#define DISCRETE_MEDIAN_HH
#include<vector>
#include<utility>
#include<stdexcept>
#include "Optional.hh"

namespace htspp {

template<typename TYPE>
class DiscreteMedian {
    private:
       typedef TYPE unit_t;
       typedef unsigned long count_t;
       typedef std::pair<DiscreteMedian::unit_t,DiscreteMedian::count_t> item_t;
       std::vector<DiscreteMedian::item_t> counts;
    public:
			void accept(TYPE depth) {
          DiscreteMedian::item_t pivot = std::make_pair(depth,1UL);
          auto it = std::lower_bound(counts.begin(), counts.end(), pivot,  [](item_t A, item_t B) -> bool { return A.first < B.first; } );
          if(it==counts.end() || it->first!=depth) {
             counts.insert(it,pivot);
             }
          else
             {
             it->second++;
             }
          }

			bool empty() const { return counts.empty();}

			Optional<double> median() {
								if(empty()) {
									return  make_empty_optional<double>();
									}
						    unsigned long L = 0UL;
						    size_t i=0;
						    for(i=0;i< counts.size();i++) {
						       L += counts[i].second;
						       }

						    unsigned long mid_x = L/2L;

						    unsigned long remain = mid_x;
						    for(i=0;i< counts.size();i++) {

						      DiscreteMedian::item_t& item  = counts[i];

						      if(item.second <  remain) {
						          remain-= item.second;
						          }
						    else
						          {
						          return make_optional<double>(item.first);
						          }
						     }
						   	throw std::runtime_error("should not happen");
						    }
					};

}




#endif

