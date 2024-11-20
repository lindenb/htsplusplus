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

#ifndef DISCRETE_MEDIAN_HH
#define DISCRETE_MEDIAN_HH
#include<vector>
#include<utility>
#include<algorithm>
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
          auto it = std::lower_bound(counts.begin(), counts.end(), depth,  [](item_t A, TYPE B) -> bool { return A.first < B; } );
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
									if(item.second ==  remain) {
										double v1 = item.first;
										double v2 = counts[i+1].first;
										 return make_optional<double>((v1+v2)/2.0);
										}
						      else if(item.second <  remain) {
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

