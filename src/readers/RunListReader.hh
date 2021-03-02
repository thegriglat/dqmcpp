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
};

#define RUNLISTREADER_HH
#endif