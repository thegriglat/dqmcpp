#include "Gnuplot2D.hh"

#include <algorithm>

namespace dqmcpp {
namespace writers {

void Gnuplot2D::setPalette(Palette palette) {
  _palette = palette;
  std::sort(
      _palette.begin(), _palette.end(),
      [](PaletteColor& a, PaletteColor& b) { return a.position < b.position; });
}

std::string Gnuplot2D::palette_str() const {
  std::string s = "(";
  for (unsigned int i = 0; i < _palette.size(); ++i) {
    auto& e = _palette.at(i);
    const auto palette_pos =
        getZ().min + e.position * (getZ().max - getZ().min);
    s += std::to_string(palette_pos) + " \"" + e.color + "\"";
    if (i != _palette.size() - 1)
      s += ", ";
  }
  s += ")";
  return s;
}

}  // namespace writers
}  // namespace dqmcpp