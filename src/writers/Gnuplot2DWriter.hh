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

class Gnuplot2DWriter {
 public:
  using Data2D = std::map<std::pair<std::string, std::string>, double>;
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
  std::string _output = "out";
  std::string _title = "title";
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
   * @brief Set output file
   *
   * @param outfile
   * @return Gnuplot2DWriter&
   */
  inline Gnuplot2DWriter& output(const std::string outfile = "out") {
    _output = outfile;
    return *this;
  }
  /**
   * @brief Get the Output file
   *
   * @return std::string
   */
  inline std::string getOutput(void) const { return _output; }
  /**
   * @brief Return current z tick interval
   *
   * @return double
   */
  inline double getZTick(void) const { return _cbtick; }
  /**
   * @brief Set the Palette. Position in PaletteColor are relative (in [0,1])
   *
   * @param palette
   * @return Gnuplot2DWriter&
   */
  Gnuplot2DWriter& setPalette(Palette palette);
  /**
   * @brief Set title
   *
   * @param title
   * @return Gnuplot2DWriter&
   */
  inline Gnuplot2DWriter& title(const std::string title = "out") {
    _title = title;
    return *this;
  }
  /**
   * @brief Get the Title
   *
   * @return std::string
   */
  inline std::string getTitle(void) const { return _title; }
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
  std::string palette_str() const;
};

#define GNUPLOT2DWRITER_HH
#endif