#include "ecalchannels.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../common/logging.hh"
#include "../common/common.hh"
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
        const auto stringlist = split(line, ",");
        for (int n = 0; n < stringlist.size(); n++) {
            auto token = stringlist.at(n);
            if (n < 26) {
                int *intptr = getPtr(info, n);
                *(intptr) = atoi(token.c_str());
            } else {
                if (n == 26)
                    info.det = token;
                else
                    info.crate = token;
            };
        }
        _channels.push_back(info);
    }
}
