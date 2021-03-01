#include "ecalchannels.hh"
#include <iostream>
#include <fstream>
#include <string>
#include "../logging/logging.hh"
using namespace std;

static int *getPtr(const ChannelInfo &info, const int n)
{
    return (int *)(&info) + n;
};

void ECALChannels::Init()
{
    _channels.reserve(75848);
    ifstream in("ecalchannels.dat");
    if (!in.good()) {
        ERROR("Cannot open ecalchannels.dat");
        return;
    }
    string line;
    while (!in.eof()) {
        std::getline(in, line);
        ChannelInfo info;
        // now parse the line
        std::string token;
        size_t pos = 0;
        int n = 0;
        while ((pos = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos);
            // std::cout << "token = " << token << std::endl;
            line.erase(0, pos + 1);
            if (n < 26) {
                int *intptr = getPtr(info, n);
                *(intptr) = atoi(token.c_str());
            } else {
                if (n == 26)
                    info.det = token;
                else
                    info.crate = token;
            };
            ++n;
        }
        _channels.push_back(info);
    }
}

