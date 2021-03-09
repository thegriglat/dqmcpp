#ifndef GNUPLOT2DWRITER_HH

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

class Gnuplot2DWriter {
  using Data2D = std::map<std::pair<std::string, std::string>, double>;

 public:
  struct PaletteColor {
    float zposition;
    std::string color;
  };
  typedef std::vector<PaletteColor> Palette;
  struct Axis {
    double min;
    double max;
  };

 private:
  Data2D _data;
  std::vector<std::string> _xlabels;
  std::vector<std::string> _ylabels;

  Axis _zaxis;
  double _cbtick = 1;
  Palette _palette = {{{-1, "white"}, {0.0, "yellow"}, {5.0, "red"}}};
  inline unsigned int nrows() const { return _xlabels.size(); }
  inline unsigned int ncolumns() const { return _ylabels.size(); }

  inline double get(const std::string& x, const std::string& y) const {
    return _data.at({x, y});
  }

 public:
  Gnuplot2DWriter(Data2D& data);
  friend std::ostream& operator<<(std::ostream& os, const Gnuplot2DWriter& gw);
  /**
   * @brief Set Z axis range
   *
   * @param min
   * @param max
   * @return Gnuplot2DWriter&
   */
  inline Gnuplot2DWriter& setZ(const double min = 0, const double max = 5.0) {
    _zaxis = {std::min(min, max), std::max(min, max)};
    return *this;
  }
  /**
   * @brief Set Z tick interval
   *
   * @param tick
   * @return Gnuplot2DWriter&
   */
  inline Gnuplot2DWriter& setZTick(const double tick = 1.0) {
    _cbtick = tick;
    return *this;
  }
  /**
   * @brief Return current z tick interval
   *
   * @return double
   */
  inline double getZTick(void) const { return _cbtick; }
  /**
   * @brief Set the Palette
   *
   * @param palette
   * @return Gnuplot2DWriter&
   */
  inline Gnuplot2DWriter& setPalette(Palette palette) {
    _palette = palette;
    std::sort(_palette.begin(), _palette.end(),
              [](PaletteColor& a, PaletteColor& b) {
                return a.zposition < b.zposition;
              });
    return *this;
  }
  /**
   * @brief Returns z axis range
   *
   * @return Axis
   */
  inline Axis getZ() const { return _zaxis; };
  /**
   * @brief Return current palette
   *
   * @return Palette
   */
  inline Palette palette() const { return _palette; };
  /**
   * @brief Return gnuplot's palette representation
   *
   * @return std::string
   */
  inline std::string palette_str() const {
    std::string s = "(";
    for (int i = 0; i < _palette.size(); ++i) {
      auto& e = _palette.at(i);
      s += std::to_string(e.zposition) + " \"" + e.color + "\"";
      if (i != _palette.size() - 1)
        s += ", ";
    }
    s += ")";
    return s;
  }
};

#define GNUPLOT2DWRITER_HH
#endif