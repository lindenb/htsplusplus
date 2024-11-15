#ifndef DISCRETE_MEDIAN_HH
#define DISCRETE_MEDIAN_HH
#include<vector>
#include<utility>
#include "Optional.hh"

namespace htspp {

class DiscreteMedian {
    private:
       typedef int unit_t;
       typedef unsigned long count_t;
       typedef std::pair<DiscreteMedian::unit_t,DiscreteMedian::count_t> item_t;
       std::vector<DiscreteMedian::item_t> counts;
    public:
       void accept(DiscreteMedian::unit_t depth);
       OptionalDouble median();
    };

}

#endif

