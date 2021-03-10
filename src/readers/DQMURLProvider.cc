/**
 * @file DQMURLProvider.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief DQMURLProvider source
 */
#include "DQMURLProvider.hh"

#include <iomanip>
#include <sstream>
#include <string>

#include <iostream>

#define DQMONLINEURL "https://cmsweb.cern.ch/dqm/online/jsonfairy/archive"
#define DQMOFFLINEURL "https://cmsweb.cern.ch/dqm/offline/jsonfairy/archive"

#define ONLINEDQMDS "Global/Online/ALL"

using namespace std;

/**
 * @brief Replace specific symbols in url
 *
 * @param s
 * @return std::string
 */
std::string urlencode(const std::string value) {
  ostringstream escaped;
  escaped.fill('0');
  escaped << hex;
  for (auto c : value) {
    // Keep alphanumeric and other accepted characters intact
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' ||
        c == '/' || c == '+') {
      escaped << c;
      continue;
    }
    // Any other characters are percent-encoded
    escaped << uppercase;
    escaped << '%' << setw(2) << int((unsigned char)c);
    escaped << nouppercase;
  }
  return escaped.str();
}

namespace DQMURL {
std::string dqmurl(const unsigned int run,
                   const std::string dataset,
                   const std::string plotname) {
  const std::string ds = (dataset == "online") ? "Global/Online/ALL" : dataset;
  std::string s = (dataset == "online") ? DQMONLINEURL : DQMOFFLINEURL;
  s += "/" + std::to_string(run) + "/" + ds + "/" + urlencode(plotname);
  return s;
}
}  // namespace DQMURL