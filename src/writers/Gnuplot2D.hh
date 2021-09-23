#ifndef GNUPLOT2D_HH
/**
 * @file Gnuplot2D.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Base class for gnuplot output
 *
 *
 */
#include <map>
#include <vector>
#include "../colors/Colors.hh"
#include "Gnuplot.hh"

namespace dqmcpp {
namespace writers {

/**
 * @brief Alias for Vector of dqmcpp::colors::PaletteColor
 *
 */
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
  /**
   * @brief Alias for axis pair
   *
   */
  using Data2DAxis = std::pair<std::string, std::string>;
  /**
   * @brief Alias for map
   *
   */
  using Data2D = std::map<Data2DAxis, double>;
  /**
   * @brief Set Z axis
   *
   * @param min number or "*"
   * @param max number or "*"
   */
  inline void setZ(const std::string& min, const std::string& max) {
    _zaxis = {min, max};
  }
  /**
   * @brief Set Z axis
   *
   * @param z Axis object
   */
  inline void setZ(const Axis& z) { _zaxis = z; }
  /**
   * @brief Get Z axis
   *
   * @return Axis
   */
  inline Axis getZ() const { return _zaxis; };
  /**
   * @brief Set the Palette object
   *
   * @param palette Palette
   */
  void setPalette(const Palette& palette);
  /**
   * @brief Get the Palette object
   *
   * @return Palette
   */
  inline Palette getPalette() const { return _palette; };
  /**
   * @brief Get Palette gnuplot representation
   *
   * @return std::string
   */
  std::string palette_str() const;
  /**
   * @brief Set size of Z ticks
   *
   * @param tick tick size
   */
  inline void setZTick(const double tick) { _cbtick = tick; }
  /**
   * @brief Get size of Z ticks
   *
   * @return double
   */
  inline double getZTick(void) const { return _cbtick; }
  /**
   * @brief Set Z axis
   *
   * @param min number
   * @param max number
   */
  inline void setZ(const double min, const double max) {
    _zaxis = axis(min, max);
  }
  /**
   * @brief Set the gnuplot logscale
   *
   * @param ls gnuplot logscale setting
   */
  inline void setLogscale(const std::string& ls) { logscale = ls; }
  /**
   * @brief Get the gnuplot logscale
   *
   * @return std::string
   */
  inline std::string getLogscale() const { return logscale; }
};

}  // namespace writers
}  // namespace dqmcpp

#define GNUPLOT2D_HH
#endif  // !GNUPLOT2D_HH
