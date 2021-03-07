#include "Gnuplot2DWriter.hh"

#include "../common/common.hh"
#include <iostream>
#include <algorithm>

#define DEFAULT_VALUE (0)

Gnuplot2DWriter::Gnuplot2DWriter(Data2D &data):
    _data(data)
{
    for (auto &elem: data) {
        const auto xlabel = elem.first.first;
        const auto ylabel = elem.first.second;
        if (!has(_xlabels, xlabel)) {
            _xlabels.push_back(xlabel);
        }
        if (!has (_ylabels, ylabel)) {
            _ylabels.push_back(ylabel);
        }
    }
    // TODO: sort?
    std::sort(_xlabels.begin(), _xlabels.end());
    std::sort(_ylabels.begin(), _ylabels.end());
}

std::ostream &operator<<(std::ostream &os, const Gnuplot2DWriter &gw)
{

    // prepare default map

    double * all = new double[gw.nrows() * gw.ncolumns()];
    auto index = [&gw](const int x, const int y) {
        return gw.ncolumns() * x + y;
    };
    for (int i = 0; i < gw.ncolumns() * gw.nrows(); ++i) {
        all[i] = DEFAULT_VALUE;
    }
    for (int iy = 0; iy < gw._ylabels.size(); ++iy) {
        const auto ylabel = gw._ylabels.at(iy);
        for (int ix = 0; ix < gw._xlabels.size(); ++ix) {
            const auto xlabel = gw._xlabels.at(ix);
            if (gw._data.find({xlabel, ylabel}) != gw._data.end())
                all[index(ix, iy)] = gw.get(xlabel, ylabel);
        }
    }
    // print x labels
    os << "N ";
    for (auto &e: gw._xlabels)
        os << e << " ";
    os << std::endl;
    for (int iy = 0; iy < gw.ncolumns(); ++iy) {
        const auto ylabel = gw._ylabels.at(iy);
        os << "\"" << ylabel << "\" ";
        for (int ix = 0; ix < gw.nrows(); ++ix) {
            os << all[index(ix, iy)] << " ";
        }
        os << std::endl;
    }
    os << std::endl;
    delete[] all;
    return os;
}
