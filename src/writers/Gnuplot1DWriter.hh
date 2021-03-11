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
  const Data1D* _data = nullptr;

 public:
  Gnuplot1DWriter(const Data1D& data) : _data(&data){};
  friend std::ostream& operator<<(std::ostream& os, const Gnuplot1DWriter& gw);
};

}  // namespace writers
}  // namespace dqmcpp
#define GNUPLOT1DWRITER_HH
#endif  // !GNUPLOT1DWRITER_HH