#ifndef GNUPLOT2D_HH

#include <map>
#include <vector>
#include "Gnuplot.hh"

namespace dqmcpp {
namespace writers {

struct PaletteColor {
  float position;
  std::string color;
};

using Palette = std::vector<PaletteColor>;

class Gnuplot2D : public Gnuplot {
 private:
  Axis _zaxis;
  double _cbtick = 1;
  Palette _palette = {{{-1, "white"}, {0.0, "yellow"}, {5.0, "red"}}};

 public:
  using Data2D = std::map<std::pair<std::string, std::string>, double>;

  inline void setZ(const double min, const double max) {
    _zaxis = {std::min(min, max), std::max(min, max)};
  }
  inline Axis getZ() const { return _zaxis; };

  void setPalette(Palette palette);
  inline Palette getPalette() const { return _palette; };
  std::string palette_str() const;

  inline void setZTick(const double tick) { _cbtick = tick; }
  inline double getZTick(void) const { return _cbtick; }
};

}  // namespace writers
}  // namespace dqmcpp

#define GNUPLOT2D_HH
#endif  // !GNUPLOT2D_HH