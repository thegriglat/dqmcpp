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
 * @brief Filter for Vlasov's table
 *
 */
class Vlasov {
 private:
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
  Vlasov();
  ~Vlasov(){};
  /**
   * @brief return True if channel in Vlasov's table
   *
   * @param channel ECAL::Channel object
   * @return true
   * @return false
   */
  bool operator()(const ECAL::Channel& channel) const;
  /**
   * @brief Return True if CCU in Vlasov's table
   *
   * @param ccu CCU to check. TT == CCU for ECAL EB
   * @return true
   * @return false
   */
  bool operator()(const ECAL::CCU& ccu) const;
};

}  // namespace filters
}  // namespace dqmcpp

#define VLASOVFILTER_HH
#endif