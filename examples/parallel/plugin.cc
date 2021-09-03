#include "plugin.hh"
#include <iostream>
#include <vector>

// header for all common template functions
#include "common/common.hh"

REGISTER_PLUGIN(ParallelEx, parallel)

using namespace std;
using namespace dqmcpp;

namespace dqmcpp {

namespace plugins {

void ParallelEx::Process() {

  /** DQMCpp provides one function for parallel processing
   * foreach_mt(begin, end, function)
   * by default it uses as much CPU as possible
   *
   * !!! DQMCpp DON'T CONTROL DATA RACES !!!
   */

  // create simple vector to see how it works
  vector<int> vec;
  for (int i = 0; i < 11; ++i)
    vec.push_back(i);

  cout << "Initial values" << endl;
  for (auto &e : vec)
    cout << e << endl;

  cout << "Parallel print, should be messed up" << endl;
  // should print numbers in random order
  common::foreach_mt(vec.begin(), vec.end(),
                     [](const int &i) { cout << i << endl; });
}
} // namespace plugins

} // namespace dqmcpp
