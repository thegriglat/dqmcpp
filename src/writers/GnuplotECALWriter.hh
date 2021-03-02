#ifndef GNUPLOTECALWRITER_HH

#include <iostream>
#include <algorithm>
#include "../dataclasses/ECALHardware.hh"

/**
 * @brief Wrapper class for plotting RunData to gnuplot
 * Don't own any resources but use pointer to constructor parameter
 * It's just overloads operator<<()
 */
class GnuplotECALWriter
{
public:
    struct PaletteColor {
        float zposition;
        std::string color;
    };
    typedef std::vector<PaletteColor> Palette;
    struct ZLimits {
        float min = 0.0;
        float max = 5.0;
    };

private:
    std::vector<ECALHardware::RunData> *rd = nullptr;

    ZLimits _zrange;
    Palette _palette = {{{-1, "white"}, {0.0, "yellow"}, {5.0, "red"}}};

public:
    GnuplotECALWriter(std::vector<ECALHardware::RunData> &rundata) : rd(&rundata) {};
    /**
     * @brief Set the zrange values
     *
     * @param min
     * @param max
     * @return GnuplotECALWriter&
     */
    inline GnuplotECALWriter &setZrange(const float min = 0, const float max = 5.0)
    {
        _zrange.max = max;
        _zrange.min = min;
        return *this;
    }
    /**
     * @brief Set the Palette
     *
     * @param palette
     * @return GnuplotECALWriter&
     */
    inline GnuplotECALWriter &setPalette(Palette palette)
    {
        _palette = palette;
        std::sort(_palette.begin(), _palette.end(), [](PaletteColor &a, PaletteColor &b) {
            return a.zposition < b.zposition;
        });
        return *this;
    }
    inline ZLimits zrange() const
    {
        return _zrange;
    }
    inline Palette palette() const
    {
        return _palette;
    }
    /**
     * @brief Gets gnuplot string representation of the palette
     *
     * @return std::string
     */
    inline std::string palette_str() const
    {
        std::string s = "(";
        for (int i = 0; i < _palette.size(); ++i) {
            auto &e = _palette.at(i);
            s += std::to_string(e.zposition) + "\"" + e.color + "\"";
            if (i != _palette.size() - 1)
                s += ", ";
        }
        s += ")";
        return s;
    }
    /**
     * @brief Overload operator for std::ostream
     *
     * @param os
     * @param gw
     * @return std::ostream&
     */
    friend std::ostream &operator<<(std::ostream &os, const GnuplotECALWriter &gw);
};

#define GNUPLOTECALWRITER_HH
#endif