/**
 * @file onlineRuns.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Get run numbers from online DQM
 */
#include <iostream>
#include "../net/DQMURLProvider.hh"

using namespace std;
using namespace dqmcpp;

int main() {
  auto runs = net::DQMURL::runs_online();
  for (auto& r : runs) {
    std::cout << r.runnumber << std::endl;
  }
  return 0;
}