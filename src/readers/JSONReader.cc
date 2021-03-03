#include "JSONReader.hh"
#include "../common/logging.hh"

#include <vector>
nlohmann::json JSONReader::parseJSON(const std::string &content)
{
    auto j = nlohmann::json::parse(content);
    return j;
}

bool JSONReader::isValid(nlohmann::json &j)
{
    const auto hist = j["hist"];
    // {"hist": "unsupported type"}
    if (hist.empty() || hist.is_string())
        return false;
    return true;
}
/**
 * @brief Get iz of ECAL from histo title
 *
 * @param title
 */
static ECALHardware::DETECTORS getECALDetector(const std::string &title)
{
    if (title.find("EB") != title.npos) {
        return ECALHardware::DETECTORS::EB;
    }
    if (title.find("EE+") != title.npos) {
        return ECALHardware::DETECTORS::EEPLUS;
    }
    if (title.find("EE-") != title.npos) {
        return ECALHardware::DETECTORS::EEMINUS;
    }
    WARNING("Cannot determine ECAL iz from title");
    return ECALHardware::DETECTORS::EB;
}

std::vector<ECALHardware::ChannelData> JSONReader::parse(nlohmann::json &j)
{
    using namespace std;
    using BinContentList = vector<vector<double>>;
    // check validity of json
    if (!isValid(j))
        return std::vector<ECALHardware::ChannelData>();
    const auto iz = getECALDetector(j["hist"]["title"].get<string>());
    const auto xaxis = j["hist"]["xaxis"];
    const auto yaxis = j["hist"]["yaxis"];
    const auto xtitle = xaxis["title"].get<string>();
    const auto xnbins = xaxis["last"]["id"].get<int>() - xaxis["first"]["id"].get<int>() + 1;
    const auto xfirst = xaxis["first"]["value"].get<int>();
    const auto xlast = xaxis["last"]["value"].get<int>();
    const char xsign = (xtitle.find('-') != xtitle.npos) ? -1 : 1;
    const auto xstep = /*xsign **/ ((xlast > xfirst) ? 1 : -1);

    const auto ytitle = yaxis["title"].get<string>();
    const auto ynbins = yaxis["last"]["id"].get<int>() - yaxis["first"]["id"].get<int>() + 1;
    const auto yfirst = yaxis["first"]["value"].get<int>();
    const auto ylast = yaxis["last"]["value"].get<int>();
    const auto ystep = (ylast > yfirst) ? 1 : -1;

    // bin increment always == 1 as it is channel
    const auto content = j["hist"]["bins"]["content"].get<BinContentList>();
    std::vector<ECALHardware::ChannelData> channel_data;
    channel_data.reserve(ECALHardware::NEBChannels); // as max of EB and EE+- n channels
    int ybin = 0;
    for (int iy = yfirst; iy != ylast; iy += ystep) {
        int xbin = 0;
        for (int ix = xfirst; ix != xlast; ix += xstep) {
            const auto value = content.at(ybin).at(xbin);
            ECALHardware::Channel c;
            if (xtitle != "ix") {
                // in case of barrel we have to swap x and y values
                // horrible....
                c.ix_iphi = std::abs(iy) - ((yfirst < 0) ? 1 : 0);
                c.iy_ieta = ix * xsign;
            } else {
                c.ix_iphi = ix;
                c.iy_ieta = iy;
            }
            c.iz = iz;
            ECALHardware::ChannelData cd;
            cd.channel = c;
            cd.value = value;
            if (value != 0) {
                // to avoid overlapping DQM output for different SM
                channel_data.push_back(cd);
            }
            xbin++;
        }
        ybin++;
    };
    return channel_data;
}