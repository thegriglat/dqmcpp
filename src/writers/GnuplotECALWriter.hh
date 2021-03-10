#ifndef GNUPLOTECALWRITER_HH
/**
 * @file GnuplotECALWriter.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Write ECAL plots in gnuplot format
 */
#include <algorithm>
#include <iostream>
#include "../dataclasses/ECAL.hh"

class GnuplotECALWriter {
 public:
  struct PaletteColor {
    float zposition;
    std::string color;
  };
  typedef std::vector<PaletteColor> Palette;
  struct ZLimits {
    float min = 0.0;
    float max = 5.0;
  };

 private:
  std::vector<ECAL::RunData>* rd = nullptr;

  ZLimits _zrange;
  Palette _palette = {{{-1, "white"}, {0.0, "yellow"}, {5.0, "red"}}};

 public:
  GnuplotECALWriter(std::vector<ECAL::RunData>& rundata) : rd(&rundata){};
  /**
   * @brief Set the zrange values from `min` to `max`. Returns object itself so
   * can be chained.
   *
   * @param min Z axis lower bound
   * @param max Z axis upper bound
   * @return GnuplotECALWriter&
   */
  inline GnuplotECALWriter& setZrange(const float min = 0,
                                      const float max = 5.0) {
    _zrange.max = max;
    _zrange.min = min;
    return *this;
  }
  /**
   * @brief Set the Palette. Returns object itself so can be chained.
   *
   * @param palette Palette vector (straightforward to gnuplot)
   * @return GnuplotECALWriter&
   */
  GnuplotECALWriter& setPalette(Palette palette);
  /**
   * @brief Returns Z axis range
   *
   * @return ZLimits
   */
  inline ZLimits zrange() const { return _zrange; }
  /**
   * @brief Returns palette
   *
   * @return Palette
   */
  inline Palette palette() const { return _palette; }
  /**
   * @brief Gets gnuplot string representation of the palette
   *
   * @return std::string
   */
  std::string palette_str() const;
  /**
   * @brief Overload operator for std::ostream
   *
   * @param os
   * @param gw
   * @return std::ostream&
   */
  friend std::ostream& operator<<(std::ostream& os,
                                  const GnuplotECALWriter& gw);
};

#define GNUPLOTECALWRITER_HH
#endif