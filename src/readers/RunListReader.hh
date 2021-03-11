#ifndef RUNLISTREADER_HH
/**
 * @file RunListReader.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Parsing input file
 */
#include <iostream>
#include <vector>
#include "../ECAL/ECAL.hh"

class RunListReader {
 private:
  std::vector<ECAL::Run> _runs;

 public:
  RunListReader(const std::string filename);
  /**
   * @brief Returns (run, dataset) vector
   *
   * @return std::vector<ECAL::Run>
   */
  inline std::vector<ECAL::Run> runs() const { return _runs; }
};

#define RUNLISTREADER_HH
#endif