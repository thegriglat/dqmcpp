/**
 * @file Gnuplot2D.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 *
 */
#include "Gnuplot2D.hh"

#include <algorithm>

namespace dqmcpp {
namespace writers {

using colors::PaletteColor;
void Gnuplot2D::setPalette(const Palette& palette) {
  _palette = palette;
  std::sort(_palette.begin(), _palette.end(),
            [](const PaletteColor& a, const PaletteColor& b) {
              return a.position < b.position;
            });
}

std::string Gnuplot2D::palette_str() const {
  std::string s = "(";
  for (unsigned int i = 0; i < _palette.size(); ++i) {
    auto& e = _palette.at(i);
    const auto palette_pos =
        getZ().mind() + e.position * (getZ().maxd() - getZ().mind());
    s += std::to_string(palette_pos) + " \"" + e.color + "\"";
    if (i != _palette.size() - 1)
      s += ", ";
  }
  s += ")";
  return s;
}

}  // namespace writers
}  // namespace dqmcpp
