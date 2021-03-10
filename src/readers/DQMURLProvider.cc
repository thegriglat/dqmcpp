/**
 * @file DQMURLProvider.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief DQMURLProvider source
 */
#include "DQMURLProvider.hh"

#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include "../common/common.hh"
#include "../readers/JSONReader.hh"
#include "../readers/URLCache.hh"

#define DQMONLINEURL "https://cmsweb.cern.ch/dqm/online/jsonfairy/archive"
#define DQMOFFLINEURL "https://cmsweb.cern.ch/dqm/offline/jsonfairy/archive"

#define ONLINEDQMDS "Global/Online/ALL"

using namespace std;

namespace {
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

/**
 * @brief returns number of different symbols
 *
 * @param s1 string
 * @param s2 string
 * @return int
 */
int maxDifference(const string& s1, const string& s2) {
  const int s1len = s1.length();
  const int s2len = s2.length();
  const int minlen = std::min(s1len, s2len);
  int ndiff = 0;
  for (int pos = 0; pos < minlen; ++pos)
    if (s1.at(pos) != s2.at(pos))
      ndiff++;
  ndiff += std::max(s1len, s2len) - minlen;
  return ndiff;
}

/**
 * @brief Groups strings based on max difference
 *
 * @param list list of std::string
 * @return vector<vector<string>>
 */
vector<vector<string>> groups(const vector<string>& list, const int maxDiff) {
  vector<vector<string>> groups;
  vector<string> skiplist;
  for (auto it = list.begin(); it != list.end(); ++it) {
    vector<string> group;
    for (auto jit = it; jit != list.end(); ++jit) {
      if (has(skiplist, *jit))
        continue;
      const auto it_vlist = split(*it, "/");
      const auto jit_vlist = split(*jit, "/");
      const auto it_dataset_part = it_vlist.at(1);
      const auto jit_dataset_part = jit_vlist.at(1);
      const auto diff = maxDifference(*it, *jit);

      if (diff <= maxDiff && it_dataset_part == jit_dataset_part) {
        group.push_back(*jit);
        skiplist.push_back(*jit);
      }
    }
    groups.push_back(group);
  }
  // remove empty groups
  groups.erase(std::remove_if(groups.begin(), groups.end(),
                              [](const vector<string>& group) {
                                return group.size() == 0;
                              }),
               groups.end());
  return groups;
};

/**
 * @brief Get the last alphabetically sorted element
 *
 * @param groups vector of vector of strings
 * @return vector<string>
 */
vector<string> getLast(vector<vector<string>>& groups) {
  vector<string> result;
  for (auto& grs : groups) {
    std::sort(grs.begin(), grs.end());
    result.push_back(grs.back());
  }
  return result;
}
}  // namespace

namespace DQMURL {
std::string dqmurl(const unsigned int run,
                   const std::string dataset,
                   const std::string plotname) {
  const std::string ds = (dataset == "online") ? ONLINEDQMDS : dataset;
  std::string s = (dataset == "online") ? DQMONLINEURL : DQMOFFLINEURL;
  s += "/" + std::to_string(run) + "/" + ds + "/" + urlencode(plotname);
  return s;
}

// TODO: check if DQM session can expire
std::vector<std::string> datasets(const unsigned int run,
                                  const std::string mask,
                                  const bool useLast) {
  URLCache cache;
  const std::string query =
      "https://cmsweb.cern.ch/dqm/offline/session/EsOFGH/modify?vary=any&pat=" +
      std::to_string(run);
  auto content = cache.get(query);

  // erase first and last ( )
  content.erase(0, 1);
  content.erase(content.size() - 1, 1);
  // replace ' with "
  for (auto& c : content)
    if (c == '\'')
      c = '\"';

  auto json_content = JSONReader::parseJSON(content);
  auto j = json_content[1];
  auto j1 = j["items"][0]["items"];
  vector<string> datasets;
  for (auto& e : j1) {
    auto ds = e["dataset"].get<string>();
    datasets.push_back(ds);
  }

  // filter datasets by mask
  std::regex self_regex(
      mask, std::regex_constants::ECMAScript | std::regex_constants::icase);
  vector<string> filtered_datasets;
  filtered_datasets.reserve(datasets.size());
  for (auto& dataset : datasets) {
    if (std::regex_search(dataset, self_regex)) {
      filtered_datasets.push_back(dataset);
    }
  }

  // grouping
  if (useLast) {
    // 1 is max diff in symbols for dataset part, e.g v1, v2, v3, ...
    auto grs = groups(filtered_datasets, 1);
    filtered_datasets = getLast(grs);
  }

  return filtered_datasets;
}
}  // namespace DQMURL