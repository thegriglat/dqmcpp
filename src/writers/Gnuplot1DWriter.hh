#ifndef GNUPLOT1DWRITER_HH

#include <algorithm>
#include <map>
#include <string>
#include <vector>
#include "Gnuplot.hh"

namespace dqmcpp {
namespace writers {

/**
 * @brief Write one-dimensional gnuplot plot
 *
 */
class Gnuplot1DWriter : public Gnuplot {
 public:
  using Data1D = std::vector<std::pair<std::string, double>>;

 private:
  Data1D _data;
  std::string with_option = "boxes";

 public:
  Gnuplot1DWriter(const Data1D& data) : _data(data){};
  template <typename T, typename BinaryOp>
  Gnuplot1DWriter(const std::vector<T>& list, BinaryOp binaryOp) {
    _data.resize(list.size());
    std::transform(list.begin(), list.end(), _data.begin(), binaryOp);
  }
  /**
   * @brief Append "with" gnuplot keyword
   *
   * @param with
   */
  inline void with(const std::string& with) { with_option = with; }
  friend std::ostream& operator<<(std::ostream& os, const Gnuplot1DWriter& gw);
};

}  // namespace writers
}  // namespace dqmcpp
#define GNUPLOT1DWRITER_HH
#endif  // !GNUPLOT1DWRITER_HH