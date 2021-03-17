#ifndef GNUPLOT1DWRITER_HH

#include <map>
#include <string>
#include <vector>
#include "Gnuplot.hh"

namespace dqmcpp {
namespace writers {

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
    Data1D d;
    d.reserve(list.size());
    for (auto& e : list)
      d.push_back(binaryOp(e));
    _data = d;
  }
  inline void with(const std::string& with) { with_option = with; }
  friend std::ostream& operator<<(std::ostream& os, const Gnuplot1DWriter& gw);
};

}  // namespace writers
}  // namespace dqmcpp
#define GNUPLOT1DWRITER_HH
#endif  // !GNUPLOT1DWRITER_HH