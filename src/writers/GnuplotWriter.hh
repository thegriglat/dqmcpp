#ifndef GNUPLOTWRITER_HH

#include "../dataclasses/ECALHardware.hh"

/**
 * @brief Wrapper class for plotting RunData to gnuplot
 * Don't own any resources but use pointer to constructor parameter
 * It's just overloads operator<<()
 */
class GnuplotWriter
{
private:
    ECALHardware::RunData *rd = nullptr;

public:
    GnuplotWriter(ECALHardware::RunData &rundata) : rd(&rundata) {};
};

#define GNUPLOTWRITER_HH
#endif