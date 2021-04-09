/**
 * @file Colors.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Color definitions
 *
 * See https://www.color-hex.com/
 */
#include "Colors.hh"

namespace dqmcpp {
namespace colors {

namespace ColorSets {

const HEXColor red = "#ff280a";
const HEXColor white = "white";
const HEXColor green = "#148314";
const HEXColor yellow = "#FFFACD";
const HEXColor blue = "#2249aa";
const HEXColor orange = "#f99314";
const HEXColor gray = "#e5e5e5";
const HEXColor black = "#0e1111";

}  // namespace ColorSets

namespace PaletteSets {

const Palette Heatmap = {{0.0, ColorSets::white},
                         {0.25, "#fdc70c"},
                         {0.5, "#f3903f"},
                         {0.75, "#ed683c"},
                         {1.0, colors::ColorSets::red}};
const Palette RMSHeatMap = {{0.0, ColorSets::white},  {0.0, ColorSets::blue},
                            {0.2, ColorSets::white},  {0.4, ColorSets::yellow},
                            {0.6, ColorSets::orange}, {1.0, ColorSets::red}};

const Palette Rainbow = {
    {0.0, "white"},    {0.0, "violet"},    {1. / 6, "blue"},   {2. / 6, "cyan"},
    {3. / 6, "green"}, {4. / 6, "yellow"}, {5. / 6, "orange"}, {1.0, "red"}};

const Palette MeanHeatMap = {
    {0.0, ColorSets::black},          {0.0, ColorSets::blue},
    {0.5, ColorSets::white},          {250. / 300., ColorSets::white},
    {250. / 300., ColorSets::yellow}, {1.0, ColorSets::red}};

}  // namespace PaletteSets

}  // namespace colors
}  // namespace dqmcpp