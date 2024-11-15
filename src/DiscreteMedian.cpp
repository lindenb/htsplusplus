#include <algorithm>
#include "DiscreteMedian.hh"

using namespace std;
using namespace htspp;

void DiscreteMedian::accept(int depth) {
          count_depth_t pivot = make_pair(depth,1UL);
          auto it = std::lower_bound(counts.begin(), counts.end(), pivot, pair_cmp);
          if(it==counts.end() || it->first!=depth) {
             counts.insert(it,pivot);
             }
          else
             {
             it->second++;
             }
          }

bool DiscreteMedian::empty() { return counts.empty();}

double DiscreteMedian::median() {
          unsigned long L = 0UL;
          size_t i=0;
          for(i=0;i< counts.size();i++) {
             L += counts[i].second;
             }
             fprintf(stderr,"SUM=%d\n",L);
          unsigned long mid_x = L/2L;
             fprintf(stderr,"mid=%d\n",mid_x);
          unsigned long remain = mid_x;
          for(i=0;i< counts.size();i++) {
                       fprintf(stderr,"remain=%d\n",remain);
            count_depth_t& item  = counts[i];
                       fprintf(stderr,"remain=%d [DP=%d,N=%d]\n",remain,item.first,item.second);
            if(item.second <  remain) {
                remain-= item.second;
                }
          else
                {
                return item.first;
                }
           }
          return 0;
          }
