#ifndef COLORS_HH
/**                                                \
 * @file Colors.hh                                 \
 * @author Grigory Latyshev (thegriglat@gmail.com) \
 * @brief Colors and palettes declarations         \
 */
#include <string>
#include <vector>

namespace dqmcpp {
namespace colors {

using HEXColor = std::string;
/**
 * @brief Palette struct. Position is relative to [0,1]. HEXColor is std::string
 *
 */
struct PaletteColor {
  double position;
  HEXColor color;
};

typedef std::vector<PaletteColor> Palette;

/**
 * @brief Predefined colors
 *
 */
namespace ColorSets {
extern const HEXColor red;
extern const HEXColor white;
extern const HEXColor green;
extern const HEXColor yellow;
extern const HEXColor blue;
extern const HEXColor orange;
extern const HEXColor gray;
extern const HEXColor black;

}  // namespace ColorSets

/**
 * @brief Predefined palettes
 *
 */
namespace PaletteSets {

extern const Palette Heatmap;
extern const Palette RMSHeatMap;
extern const Palette MeanHeatMap;
extern const Palette Rainbow;

}  // namespace PaletteSets

}  // namespace colors
}  // namespace dqmcpp

#define COLORS_HH
#endif