/**
 * @file RunListReader.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief RunListReader code
 */
#include "RunListReader.hh"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include "../ECAL/ECAL.hh"
#include "../common/common.hh"

using namespace std;

namespace {
bool runSorter(const dqmcpp::ECAL::Run& a, const dqmcpp::ECAL::Run& b) {
  return (a.runnumber < b.runnumber);
}

std::pair<bool, dqmcpp::ECAL::Run> parse(std::string line) {
  auto pos = line.find("#");
  if (pos != line.npos)
    line.erase(pos);
  line = dqmcpp::common::trim(line);
  auto tokens = dqmcpp::common::split(line);
  if (tokens.size() == 0 || line.size() == 0) {
    // dummy output
    return {false, dqmcpp::ECAL::Run(0, "")};
  }
  const auto runnumber = atoi(tokens.at(0).c_str());
  if (tokens.size() == 1) {
    cout << "No dataset specified for run '" << runnumber
         << "'. Online DQM is assumed." << endl;
    return {true, dqmcpp::ECAL::Run(runnumber, "online")};
  }
  return {true, dqmcpp::ECAL::Run(runnumber, tokens.at(1))};
}

}  // namespace

namespace dqmcpp {
namespace readers {

RunListReader::RunListReader(const std::string& filename) {
  if (filename == "-") {
    // stdin
    std::string line;

    while (std::getline(std::cin, line)) {
      auto r = parse(line);
      if (r.first)
        _runs.push_back(r.second);
    }
    std::sort(_runs.begin(), _runs.end(), runSorter);
    return;
  }
  ifstream in(filename);
  if (!in.good()) {
    std::cout << "Cannot open runlist file" << std::endl;
    exit(1);
  }
  string line;
  while (!in.eof()) {
    std::getline(in, line);
    // strip # and everything after
    auto r = parse(line);
    if (r.first)
      _runs.push_back(r.second);
  };
  in.close();
  std::sort(_runs.begin(), _runs.end(), runSorter);
}

}  // namespace readers
}  // namespace dqmcpp