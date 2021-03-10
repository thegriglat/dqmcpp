/**
 * @file findDataset.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Find and output Offline DQM dataset for given list of runs
 */
#include <cstdlib>
#include <iostream>
#include <string>
#include "../readers/DQMURLProvider.hh"

using namespace std;

void help(const string& progname) {
  cout << "Usage: <runs> | " << progname << " <Dataset mask>" << endl
       << "  Dataset mask supports regexp." << endl;
}

int main(int argc, char** argv) {
  if (argc == 2 && (argv[1] == "-h" || argv[1] == "--help")) {
    help(argv[0]);
    return 0;
  }
  const string mask = (argc == 1) ? ".*" : argv[1];
  string line;
  while (std::getline(cin, line)) {
    int run = std::atoi(line.c_str());
    if (run == 0) {
      cerr << "Bad run '" << line << "'" << endl;
      continue;
    }
    auto datasets = DQMURL::datasets(run, mask);
    for (int i = 0; i < datasets.size(); ++i) {
      if (i > 0)
        cout << "# ";
      cout << run << " " << datasets.at(i) << endl;
    }
  }
  return 0;
}