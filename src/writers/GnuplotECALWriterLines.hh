#ifndef GNUPLOTECALWRITERLINES_HH
/**
 * @file GnuplotECALWriterLines.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Helper class for drawing ECAL
 */
#include <array>
#include <vector>
/**
 * @brief Returns list of points for EE lines
 *
 * @return std::vector<std::array<int, 4>>
 */
std::vector<std::array<int, 4>> EndcapLines();
/**
 * @brief Draw gnuplot EE SM labels
 *
 * @param os std::ostream object
 * @param iz +1 for EE+, -1 for EE-
 */
void drawEESM(std::ostream& os, const int iz);

#define GNUPLOTECALWRITERLINES_HH
#endif