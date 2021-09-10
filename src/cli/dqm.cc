/**
 * @file dqm.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Main file for running plugins
 */
#include <fstream>
#include <iostream>
#include <vector>

#include "cxxopts/cxxopts.hpp"

#include "../common/common.hh"
#include "../plugins/Plugins.hh"
#include "../readers/JSONReader.hh"
#include "../readers/RunListReader.hh"

using namespace std;
using namespace dqmcpp;

int main(int argc, char** argv) {
  auto& factory = plugins::PluginFactory::Instance();

  // === CLI PARSER ===
  cxxopts::Options cliopts("dqm", "A PFG DQM analysis tool");

  cliopts.add_options()("h,help", "Print usage")(
      "p,plugin", "List of plugins to run | all",
      cxxopts::value<std::vector<std::string>>())(
      "r,runfile", "Runlist file | '-' for stdin",
      cxxopts::value<std::string>());
  cliopts.parse_positional({"runfile", "plugin"});
  cliopts.positional_help("runlist plugins|all");
  cliopts.show_positional_help();

  // helper function
  auto showhelp = [&cliopts, &factory]() {
    cout << cliopts.help() << endl;
    cout << "Plugins: " << endl;
    for (auto& name : factory.list()) {
      std::cout << "  " << name << std::endl;
    }
  };

  auto cli_result = cliopts.parse(argc, argv);

  if (cli_result.count("help") || cli_result.arguments().size() == 0) {
    showhelp();
    exit(0);
  }

  if (!cli_result.count("plugin")) {
    showhelp();
    cout << "Plugin to run not specified! " << endl;
    exit(0);
  }

  if (!cli_result.count("runfile")) {
    showhelp();
    cout << "Run file  not specified! " << endl;
    exit(0);
  }
  // === END CLI PARSER ===

  const std::string infile = cli_result["runfile"].as<std::string>();
  auto plugin_names = cli_result["plugin"].as<std::vector<std::string>>();

  if (common::has(plugin_names, std::string("all")))
    plugin_names = factory.list();
  for (auto plugin_name : plugin_names) {
    std::cout << "##### RUN " << plugin_name << " #####" << std::endl;
    auto plugin = factory.GetPlugin(plugin_name);
    if (!plugin) {
      exit(1);
    }
    readers::RunListReader rlr(infile);
    plugin->setRunListReader(&rlr);
    plugin->Process();
    std::cout << "##### END " << plugin_name << " #####" << std::endl;
  }
  return 0;
}