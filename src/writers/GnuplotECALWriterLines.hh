#ifndef GNUPLOTECALWRITERLINES_HH

#include <array>
#include <vector>

std::vector<std::array<int, 4>> EndcapLines();
void drawEESM(std::ostream &os, const int iz);


#define GNUPLOTECALWRITERLINES_HH
#endif