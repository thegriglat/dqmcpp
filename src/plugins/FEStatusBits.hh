#ifndef FESTATUSBITS_HH

#include "Plugins.hh"

namespace dqmcpp {
namespace plugins {

class FEStatusBits : public Plugin {
 public:
  /**
   * @brief List of statuses in DQM
   *
   */
  static const std::vector<std::string> STATUSES;
  void Process();
};

}  // namespace plugins
}  // namespace dqmcpp

#define FESTATUSBITS_HH
#endif  // !FESTATUSBITS_HH