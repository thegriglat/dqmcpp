/**
 * @file dqm.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Main file for running plugins
 */
#include <fstream>
#include <iostream>
#include <vector>
#include "../common/common.hh"
#include "../plugins/Plugins.hh"
#include "../readers/JSONReader.hh"
#include "../readers/RunListReader.hh"

using namespace std;
using namespace dqmcpp;

int main(int argc, char** argv) {
  auto& factory = plugins::PluginFactory::Instance();
  if (argc < 2 || argc > 3) {
    std::cout << "Usage: " << argv[0]
              << " plugin list|all [ <runlist file> | - ]" << std::endl;
    std::cout << "Plugins: " << std::endl;
    for (auto& name : factory.list()) {
      std::cout << "  " << name << std::endl;
    }
    return 0;
  }
  const std::string infile = (argc == 3) ? argv[2] : "-";
  auto plugin_names = common::split(argv[1], ",");

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