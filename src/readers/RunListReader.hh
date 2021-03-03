#ifndef RUNLISTREADER_HH

#include <iostream>
#include <vector>
#include "../dataclasses/ECALHardware.hh"

class RunListReader
{
private:
    std::vector<ECALHardware::Run> _runs;

public:
    RunListReader(const std::string filename);
    /**
     * @brief Returns (run, dataset) vector
     *
     * @return std::vector<ECALHardware::Run>
     */
    inline std::vector<ECALHardware::Run> runs() const
    {
        return _runs;
    }
};

#define RUNLISTREADER_HH
#endif