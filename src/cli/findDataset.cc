/**
 * @file findDataset.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Find and output Offline DQM dataset for given list of runs
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include "../net/DQMURLProvider.hh"

using namespace std;

void help(const string& progname) {
  cout << "Usage: <runs> | " << progname << " <Dataset mask>" << endl
       << "  Dataset mask supports regexp." << endl;
}

int main(int argc, char** argv) {
  std::vector<std::string> args;
  args.reserve(argc);
  for (int i = 0; i < argc; ++i) {
    args.push_back(argv[i]);
  }
  if (argc == 2 && (args.at(1) == "-h" || args.at(1) == "--help")) {
    help(args.at(0));
    return 0;
  }
  const string mask = (argc == 1) ? ".*" : args.at(1);
  string line;
  while (std::getline(cin, line)) {
    int run = std::atoi(line.c_str());
    if (run == 0) {
      cerr << "Bad run '" << line << "'" << endl;
      continue;
    }
    auto datasets = DQMURL::datasets(run, mask);
    for (unsigned int i = 0; i < datasets.size(); ++i) {
      if (i > 0)
        cout << "# ";
      cout << run << " " << datasets.at(i) << endl;
    }
  }
  return 0;
}