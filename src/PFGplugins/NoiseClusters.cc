#include "NoiseClusters.hh"
#include "RMSChannels.hh"

#include <fstream>
#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "colors/Colors.hh"
#include "common/clusters.hh"
#include "common/common.hh"
#include "ecalchannels/ECALChannels.hh"
#include "writers/Gnuplot2DWriter.hh"

REGISTER_PLUGIN(NoiseClusters);

namespace {
int chDistance2(const dqmcpp::ECAL::ChannelData& a,
                const dqmcpp::ECAL::ChannelData& b) {
  const auto dx = a.base.ix_iphi - b.base.ix_iphi;
  const auto dy = a.base.iy_ieta - b.base.iy_ieta;
  return dx * dx + dy * dy;
}
}  // namespace

void dqmcpp::plugins::NoiseClusters::Process() {
  const auto runs = runListReader->runs();
  RMSChannels rmsplugin;
  rmsplugin.setRunListReader(runListReader);
  const auto rmsdata = rmsplugin.getRunData();
  writers::Gnuplot2DWriter::Data2D data;
  int _maxsize = -1;
  for (int det = -1; det <= 1; ++det) {
    // EB == 0, EE == +-1
    const double limit = (det == 0) ? 3.5 : 5.0;
    std::vector<ECAL::RunChannelData> detrmsdata(rmsdata.begin(),
                                                 rmsdata.end());
    for (auto& d : detrmsdata) {
      const auto xlabel = std::to_string(d.run.runnumber);
      // remove other det
      d.data.erase(std::remove_if(d.data.begin(), d.data.end(),
                                  [det, limit](const ECAL::ChannelData& chd) {
                                    const bool wrongdet =
                                        std::abs(chd.base.iz) != det;
                                    const bool wrongrms = chd.value <= limit;
                                    return wrongdet || wrongrms;
                                  }),
                   d.data.end());
      // now we have only one detector with rms cut;
      // clusterize data
      auto clusters = common::clusters(d.data, 1, chDistance2);
      // remove all clusters with size <= 4
      clusters.erase(
          std::remove_if(clusters.begin(), clusters.end(),
                         [](const std::vector<ECAL::ChannelData>& chdv) {
                           return chdv.size() <= 4;
                         }),
          clusters.end());
      for (auto& cluster : clusters) {
        _maxsize = std::max((int)cluster.size(), _maxsize);
        std::set<int> ttlist;
        for (auto& chd : cluster) {
          const auto info = ECALChannels::find(chd.base);
          ttlist.insert(info->ccu);
        }
        std::vector<int> ttsorted(ttlist.begin(), ttlist.end());
        std::sort(ttsorted.begin(), ttsorted.end());
        std::string dstr = "EB";
        if (det == 1)
          dstr = "EE+";
        if (det == -1)
          dstr = "EE-";
        std::stringstream yss;
        yss << dstr << " ["
            << common::join(common::map<int, std::string>(
                   ttsorted, [](const int i) { return std::to_string(i); }))
            << "]";
        const std::string ylabel = yss.str();
        data.insert({{xlabel, ylabel}, cluster.size()});
      }
    }
  }
  writers::Gnuplot2DWriter writer(data);
  writer.setOutput("NoiseClusters.png");
  writer.setTitle("NoiseClusters");
  writer.setZ(0, _maxsize);
  writer.setZTick(1);
  writer.setPalette(colors::PaletteSets::Heatmap);
  writer.setXlabels(common::map<ECAL::Run, std::string>(
      runs, [](const ECAL::Run& r) { return std::to_string(r.runnumber); }));
  std::ofstream out("NoiseClusters.plt");
  out << writer;
  out.close();
}
