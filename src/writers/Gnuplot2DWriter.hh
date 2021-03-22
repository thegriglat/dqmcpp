#ifndef GNUPLOT2DWRITER_HH
/**
 * @file Gnuplot2DWriter.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Output 2D histograms
 */
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>
#include "Gnuplot2D.hh"

namespace dqmcpp {
namespace writers {

class Gnuplot2DWriter : public Gnuplot2D {
 private:
  const Data2D* _data = nullptr;
  std::vector<std::string> _xlabels;
  std::vector<std::string> _ylabels;
  std::function<bool(const std::string&, const std::string&)> sortfn = nullptr;

  inline unsigned int nrows() const { return _xlabels.size(); }
  inline unsigned int ncolumns() const { return _ylabels.size(); }

  inline double get(const std::string& x, const std::string& y) const {
    return _data->at({x, y});
  }

 public:
  Gnuplot2DWriter(const Data2D& data);
  void setSortYFn(std::function<bool(const std::string&, const std::string&)>
                      sort_function);
  friend std::ostream& operator<<(std::ostream& os, const Gnuplot2DWriter& gw);
};

}  // namespace writers
}  // namespace dqmcpp
#define GNUPLOT2DWRITER_HH
#endif