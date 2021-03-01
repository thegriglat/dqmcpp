#include "JSONReader.hh"

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
    const auto xbinfirst = xaxis["first"]["value"].get<int>();
    const auto ytitle = yaxis["title"].get<string>();
    const auto ynbins = yaxis["last"]["id"].get<int>() - yaxis["first"]["id"].get<int>() + 1;
    const auto ybinfirst = yaxis["first"]["value"].get<int>();
    // bin increment always == 1 as it is channel
    const auto content = j["hist"]["bins"]["content"].get<BinContentList>();
    const char sign = (xtitle.find('-') != xtitle.npos) ? -1 : 1;
    std::vector<ECALHardware::ChannelData> channel_data;
    channel_data.reserve(ECALHardware::NEBChannels); // as max of EB and EE+- n channels
    for (int ybin = 0; ybin < ynbins; ++ybin) {
        const auto iy = ybinfirst + ybin;
        for (int xbin = 0; xbin < xnbins; ++xbin) {
            const auto ix = xbinfirst + xbin;
            const auto value = content.at(ybin).at(xbin);
            ECALHardware::Channel c;
            c.ix_iphi = std::abs(iy);
            c.iy_ieta = ix * sign;
            c.iz = iz;
            ECALHardware::ChannelData cd;
            cd.channel = c;
            cd.value = value;
            channel_data.push_back(cd);
        }
    };
    return channel_data;
}