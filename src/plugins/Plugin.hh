#ifndef PLUGIN_HH
/**
 * @file Plugin.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Virtual class for Plugins
 */
#include <string>
#include <vector>
#include "../ECAL/ECAL.hh"
#include "../readers/JSONReader.hh"
#include "../readers/RunListReader.hh"

namespace dqmcpp {
namespace plugins {

class Plugin {
 protected:
  dqmcpp::readers::RunListReader* runListReader = nullptr;

 public:
  /**
   * @brief Main function for Plugin
   *
   */
  virtual void Process(void) = 0;

  /**
   * @brief Set the Run List Reader object
   *
   * @param runlistreader RunListReader* pointer
   */
  inline void setRunListReader(dqmcpp::readers::RunListReader* runlistreader) {
    runListReader = runlistreader;
  }
};

}  // namespace plugins
}  // namespace dqmcpp
#define PLUGIN_HH
#endif