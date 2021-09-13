/**
 * @file Vlasov.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Filter channel by Vlasov's table
 * https://twiki.cern.ch/twiki/bin/view/Main/EcalTTcurrentStatus
 *
 */

#ifndef VLASOVFILTER_HH

#include <array>
#include <vector>
#include "../ECAL/ECAL.hh"

namespace dqmcpp {

namespace filters {
/**
 * @brief Filter by Vlasov's table
 * https://twiki.cern.ch/twiki/bin/view/Main/EcalTTcurrentStatus
 *
 */
namespace Vlasov {

class VlasovTable {
 private:
  void init();

 protected:
  /**
   * @brief Vlasov's item, like std::pair but with named fields
   *
   */
  struct Item {
    int fed;
    int ccu;
  };
  std::vector<Item> _table;

 public:
  VlasovTable() { init(); };
  ~VlasovTable(){};
};

/**
 * @brief Filter channels by Vlasov table
 *
 */
class Channel : public VlasovTable {
 public:
  /**
   * @brief return True if channel in Vlasov's table
   *
   * @param channel ECAL::Channel object
   * @return true
   * @return false
   */
  bool operator()(const ECAL::Channel& channel) const;
};

/**
 * @brief Filter CCU (or TT for EB) by Vlasov table
 *
 */
class CCU : public VlasovTable {
 public:
  /**
   * @brief Return True if CCU in Vlasov's table
   *
   * @param ccu CCU to check. TT == CCU for ECAL EB
   * @return true
   * @return false
   */
  bool operator()(const ECAL::CCU& ccu) const;
};

}  // namespace Vlasov
}  // namespace filters
}  // namespace dqmcpp

#define VLASOVFILTER_HH
#endif