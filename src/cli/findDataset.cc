/**
 * @file findDataset.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Find and output Offline DQM dataset for given list of runs
 */
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
#include "../net/DQMURLProvider.hh"

using namespace std;
using namespace dqmcpp;

void help(const string& progname) {
  cout << "Usage: " << progname << " run|- DatasetMask" << endl
       << "  Dataset mask supports regexp." << endl;
}

void print_rund(const int run, const std::string& mask) {
  const auto runs = net::DQMURL::runs(run, mask);
  std::set<int> distinct_runs;
  for (auto& r : runs)
    distinct_runs.insert(r.runnumber);
  for (const auto& runnumber : distinct_runs) {
    int i = 0;
    for (const auto& rund : runs) {
      if (rund.runnumber == runnumber) {
        if (i > 0)
          cout << "# ";
        cout << rund.runnumber << " " << rund.dataset << endl;
        ++i;
      }
    }
  }
}

int main(int argc, char** argv) {
  std::vector<std::string> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  if (argc <= 2) {
    help(args.at(0));
    return 0;
  }
  const string runstr = args.at(1);
  const string mask = args.at(2);
  if (runstr == "-") {
    string line;
    while (std::getline(cin, line)) {
      const int run = std::atoi(line.c_str());
      if (run == 0) {
        cerr << "Bad run '" << line << "'" << endl;
        continue;
      }
      print_rund(run, mask);
    }
  } else {
    const int run = std::atoi(runstr.c_str());
    if (run == 0) {
      cerr << "Bad run '" << runstr << "'" << endl;
      return 1;
    }
    print_rund(run, mask);
  }
  return 0;
}