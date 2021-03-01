#ifndef GNUPLOTECALWRITER_HH

#include <iostream>
#include "../dataclasses/ECALHardware.hh"

/**
 * @brief Wrapper class for plotting RunData to gnuplot
 * Don't own any resources but use pointer to constructor parameter
 * It's just overloads operator<<()
 */
class GnuplotECALWriter
{
private:
    std::vector<ECALHardware::RunData> *rd = nullptr;

public:
    GnuplotECALWriter(std::vector<ECALHardware::RunData> &rundata) : rd(&rundata) {};
    friend std::ostream &operator<<(std::ostream &os, const GnuplotECALWriter &gw);
};

#define GNUPLOTECALWRITER_HH
#endif