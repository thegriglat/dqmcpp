#ifndef RUNLISTREADER_HH
/**
 * @file RunListReader.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Parsing input file
 */
#include <iostream>
#include <vector>
#include "../ECAL/ECAL.hh"

namespace dqmcpp {
/**
 * @brief Namespace for reading functions/classes
 *
 */
namespace readers {

/**
 * @brief Class to read and parse runfile
 *
 */
class RunListReader {
 private:
  std::vector<ECAL::Run> _runs;

 public:
  RunListReader(const std::string& filename);
  /**
   * @brief Returns (run, dataset) std::vector
   *
   * @return std::vector<ECAL::Run>
   */
  inline std::vector<ECAL::Run> runs() const { return _runs; }
};

}  // namespace readers
}  // namespace dqmcpp
#define RUNLISTREADER_HH
#endif