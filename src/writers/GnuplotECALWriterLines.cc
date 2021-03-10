/**
 * @file GnuplotECALWriterLines.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Array generation for ECAL EE +/- lines
 */
#include "GnuplotECALWriterLines.hh"

#include <cmath>
#include <iomanip>
#include <iostream>

static std::vector<std::array<int, 4>> generateLines(
    std::vector<std::array<int, 2>> points) {
  std::vector<std::array<int, 4>> r;
  for (unsigned int i = 1; i < points.size(); ++i) {
    r.push_back(
        {points[i - 1][0], points[i - 1][1], points[i][0], points[i][1]});
  }
  return r;
}

static void mirrorX(std::vector<std::array<int, 4>>& lines) {
  const auto size = lines.size();
  for (unsigned int i = 0; i < size; i++) {
    auto x1 = lines[i][0];
    auto y1 = lines[i][1];
    auto x2 = lines[i][2];
    auto y2 = lines[i][3];
    lines.push_back({100 - x1, y1, 100 - x2, y2});
  }
}

std::vector<std::array<int, 4>> EndcapLines() {
  // contour
  std::vector<std::array<int, 2>> points = {
      {{50, 0},  {40, 0},  {40, 3},  {35, 3},   {35, 5},  {25, 5},  {25, 8},
       {20, 8},  {20, 13}, {15, 13}, {15, 15},  {13, 15}, {13, 20}, {8, 20},
       {8, 25},  {5, 25},  {5, 35},  {3, 35},   {3, 40},  {0, 40},  {0, 60},
       {3, 60},  {3, 65},  {5, 65},  {5, 75},   {8, 75},  {8, 80},  {13, 80},
       {13, 85}, {15, 85}, {15, 87}, {20, 87},  {20, 92}, {25, 92}, {25, 95},
       {35, 95}, {35, 97}, {40, 97}, {40, 100}, {50, 100}}};
  auto r = generateLines(points);
  // inner lines
  for (auto& e : generateLines(
           {{{35, 3}, {35, 15}, {40, 15}, {40, 30}, {45, 30}, {45, 40}}}

           ))
    r.push_back(e);
  for (auto& e : generateLines({{{7, 25},
                                 {10, 25},
                                 {10, 30},
                                 {20, 30},
                                 {20, 35},
                                 {30, 35},
                                 {30, 40},
                                 {35, 40},
                                 {35, 45},
                                 {40, 45}}}))
    r.push_back(e);
  for (auto& e : generateLines(
           {{{0, 60}, {10, 60}, {10, 55}, {35, 55}, {35, 50}, {39, 50}}}))
    r.push_back(e);
  for (auto& e : generateLines({{{20, 87},
                                 {20, 85},
                                 {25, 85},
                                 {25, 75},
                                 {30, 75},
                                 {30, 70},
                                 {35, 70},
                                 {35, 65},
                                 {40, 65},
                                 {40, 60},
                                 {45, 60}}}))
    r.push_back(e);
  for (auto& e :
       generateLines({{{50, 39}, {45, 39}, {45, 40}, {43, 40}, {43, 41},
                       {42, 41}, {42, 42}, {41, 42}, {41, 43}, {40, 43},
                       {40, 45}, {39, 45}, {39, 55}, {40, 55}, {40, 57},
                       {41, 57}, {41, 58}, {42, 58}, {42, 59}, {43, 59},
                       {43, 60}, {45, 60}, {45, 61}, {50, 61}

       }}))
    r.push_back(e);
  mirrorX(r);
  r.push_back({50, 100, 50, 61});
  return r;
}

void drawEESM(std::ostream& os, const int iz) {
  // radius
  const int r = 35;
  const auto pi = 3.14159265359;
  const auto phi = 2 * pi / 9.0;
  // p is calculated so "+5" label (i == 5) will be at the center
  const double p = 1.5 * pi / phi - 5.;
  for (int i = 1; i < 10; ++i) {
    const auto angle = p * phi + i * phi;
    const int x = r * std::cos(angle) + 50;
    const int y = r * std::sin(angle) + 50;

    os << "set label front \"" << ((iz > 0) ? "+" : "-") << std::setw(2)
       << std::setfill('0') << i << "\" at " << x << "," << y << std::endl;
  }
}
