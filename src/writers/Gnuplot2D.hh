#ifndef GNUPLOT2D_HH

#include <map>
#include <vector>
#include "../colors/Colors.hh"
#include "Gnuplot.hh"

namespace dqmcpp {
namespace writers {

using Palette = std::vector<dqmcpp::colors::PaletteColor>;

/**
 * @brief Base class for two-dimensional gnuplot plots
 *
 */
class Gnuplot2D : public Gnuplot {
 private:
  Axis _zaxis = {"*", "*"};
  double _cbtick = 1;
  Palette _palette = dqmcpp::colors::PaletteSets::Heatmap;
  std::string logscale = "";

 public:
  using Data2D = std::map<std::pair<std::string, std::string>, double>;

  inline void setZ(const std::string& min, const std::string& max) {
    _zaxis = {min, max};
  }
  inline void setZ(const Axis& z) { _zaxis = z; }
  inline Axis getZ() const { return _zaxis; };

  void setPalette(const Palette& palette);
  inline Palette getPalette() const { return _palette; };
  std::string palette_str() const;

  inline void setZTick(const double tick) { _cbtick = tick; }
  inline double getZTick(void) const { return _cbtick; }
  inline void setZ(const double min, const double max) {
    _zaxis = axis(min, max);
  }
  inline void setLogscale(const std::string& ls) { logscale = ls; }
  inline std::string getLogscale() const { return logscale; }
};

}  // namespace writers
}  // namespace dqmcpp

#define GNUPLOT2D_HH
#endif  // !GNUPLOT2D_HH
