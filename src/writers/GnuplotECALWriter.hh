#ifndef GNUPLOTECALWRITER_HH
/**
 * @file GnuplotECALWriter.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Write ECAL plots in gnuplot format
 */
#include <algorithm>
#include <iostream>
#include "../ECAL/ECAL.hh"
#include "Gnuplot2D.hh"

namespace dqmcpp {
namespace writers {

class GnuplotECALWriter : public Gnuplot2D {
 private:
  std::vector<ECAL::RunData>* rd = nullptr;

 public:
  GnuplotECALWriter(std::vector<ECAL::RunData>& rundata) : rd(&rundata){};

  friend std::ostream& operator<<(std::ostream& os,
                                  const GnuplotECALWriter& gw);
};

}  // namespace writers
}  // namespace dqmcpp
#define GNUPLOTECALWRITER_HH
#endif