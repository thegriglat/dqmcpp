#include "RunListReader.hh"

#include <string>
#include <fstream>
#include "../common/common.hh"

using namespace std;

RunListReader::RunListReader(const std::string filename)
{
    ifstream in(filename);
    if (!in.good()) {
        std::cout << "Cannot open runlist file" << std::endl;
        exit(1);
    }
    string line;
    while (!in.eof()) {
        std::getline(in, line);
        auto tokens = split(line);
        if (tokens.size() < 2) {
            std::cout << "line '" << line << "' contains less than 2 fields. Skipping" << std::endl;
            for (auto &e: tokens)
                std::cout << "  " << e << std::endl;
            continue;
        }
        ECAL::Run r(atoi(tokens.at(0).c_str()), tokens.at(1));
        _runs.push_back(r);
    };
    in.close();
}