/**
 * @file DQMURLProvider.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief DQMURLProvider source
 */
#include "DQMURLProvider.hh"

#include <functional>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include "../common/JSONParser.hh"
#include "../common/common.hh"
#include "../readers/JSONReader.hh"
#include "DQMSession.hh"
#include "URLCache.hh"

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
std::string urlencode(const std::string& value) {
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
template <typename T>
vector<vector<T>> groups(const vector<T>& list,
                         const int maxDiff,
                         std::function<std::string(const T&)> getter) {
  vector<vector<T>> groups;
  vector<T> skiplist;
  for (auto it = list.begin(); it != list.end(); ++it) {
    const auto field_it = getter(*it);
    const auto it_vlist = dqmcpp::common::split(field_it, "/");
    const auto it_dataset_part = it_vlist.at(1);

    vector<T> group;
    for (auto jit = it; jit != list.end(); ++jit) {
      if (dqmcpp::common::has(skiplist, *jit))
        continue;
      const auto field_jit = getter(*jit);
      const auto jit_vlist = dqmcpp::common::split(field_jit, "/");
      const auto jit_dataset_part = jit_vlist.at(1);
      const auto diff = maxDifference(field_it, field_jit);

      if (diff <= maxDiff && it_dataset_part == jit_dataset_part) {
        group.push_back(*jit);
        skiplist.push_back(*jit);
      }
    }
    groups.push_back(group);
  }
  // remove empty groups
  groups.erase(
      std::remove_if(groups.begin(), groups.end(),
                     [](const vector<T>& group) { return group.size() == 0; }),
      groups.end());
  // sort
  for (auto& gr : groups)
    std::sort(gr.begin(), gr.end(), [getter](const T& a, const T& b) {
      return getter(a) < getter(b);
    });
  return groups;
};

/**
 * @brief Get the last element of each vector
 *
 * @param groups vector of vector of strings
 * @return vector<string>
 */
template <typename T>
vector<T> getLast(const vector<vector<T>>& groups) {
  vector<T> result;
  for (auto& grs : groups) {
    result.push_back(grs.back());
  }
  return result;
}

}  // namespace

namespace dqmcpp {
namespace net {
namespace DQMURL {

std::string dqmurl(const unsigned int run,
                   const std::string& dataset,
                   const std::string& plotname) {
  const std::string ds = (dataset == "online") ? ONLINEDQMDS : dataset;
  std::string s = (dataset == "online") ? DQMONLINEURL : DQMOFFLINEURL;
  s += "/" + std::to_string(run) + "/" + ds + "/" + urlencode(plotname);
  return s;
}

std::vector<ECAL::Run> runs(const unsigned int runnumber,
                            const std::string& mask,
                            const bool useLast) {
  auto session = DQMSession::get();
  // firstly we need to GET chooseSample to init ??? in DQM
  net::URLCache::get("https://cmsweb.cern.ch/dqm/offline/session/" + session +
                     "/chooseSample?vary=run;order=dataset");
  const std::string query =
      "https://cmsweb.cern.ch/dqm/offline/session/" + session +
      "/modify?vary=any;order=dataset;pat=" + std::to_string(runnumber);
  auto content = net::URLCache::get(query);

  // erase first and last ( )
  content.erase(0, 1);
  content.erase(content.size() - 1, 1);
  // replace ' with "
  for (auto& c : content)
    if (c == '\'')
      c = '\"';

  auto json_content = dqmcpp::common::parseJSON(content);
  auto& j = json_content[1];
  auto& j1 = j["items"][0]["items"];
  vector<ECAL::Run> runs;
  for (rapidjson::Value::ConstValueIterator itr = j1.Begin(); itr != j1.End();
       ++itr) {
    const std::string ds = (*itr)["dataset"].GetString();
    const std::string runstr = (*itr)["run"].GetString();
    auto run = std::atoi(runstr.c_str());
    runs.push_back(ECAL::Run(run, ds));
  }

  // filter datasets by mask
  std::regex self_regex(
      mask, std::regex_constants::ECMAScript | std::regex_constants::icase);
  vector<ECAL::Run> filtered_runs;
  filtered_runs.reserve(runs.size());
  for (auto& run : runs) {
    if (std::regex_search(run.dataset, self_regex)) {
      filtered_runs.push_back(run);
    }
  }

  // grouping
  if (useLast) {
    // 1 is max diff in symbols for dataset part, e.g v1, v2, v3, ...
    filtered_runs = getLast(groups<ECAL::Run>(
        filtered_runs, 1, [](const ECAL::Run& run) { return run.dataset; }));
  }

  return filtered_runs;
}

std::string dqmurl(const ECAL::Run& run, const std::string& plotname) {
  return dqmurl(run.runnumber, run.dataset, plotname);
}

}  // namespace DQMURL
}  // namespace net
}  // namespace dqmcpp
