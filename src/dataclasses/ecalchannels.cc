#include "ecalchannels.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../logging/logging.hh"
using namespace std;

// symbols from dumped ecalchannels.o
extern "C" char _binary_ecalchannels_dat_start;
extern "C" char _binary_ecalchannels_dat_end;

static int *getPtr(const ChannelInfo &info, const int n)
{
    return (int *)(&info) + n;
};

void ECALChannels::Init()
{
    _channels.reserve(75848);
    // read text from ecalchannels.o to stringstream
    std::stringstream in;
    char *p = &_binary_ecalchannels_dat_start;
    while (p != &_binary_ecalchannels_dat_end) {
        // just insert byte-per-byte
        in << *p++;
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
