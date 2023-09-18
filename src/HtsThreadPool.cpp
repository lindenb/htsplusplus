#include "HtsThreadPool.hh"
using namespace std;

std::unique_ptr<HtsThreadPool> HtsThreadPool::create(int nthreads) {
 	return  std::unique_ptr<HtsThreadPool>(new HtsThreadPool(nthreads));
 	}