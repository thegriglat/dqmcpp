/**
 * @file MaskedTT.сс
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief MaskedTT data and implementation
 *
 */

#include "MaskedTT.hh"
#include <algorithm>
#include <array>
#include "../ECAL/ECAL.hh"
#include "../ecalchannels/ECALChannels.hh"

/*
  SELECT BTT_ID FROM CMS_ECAL_CONF.FE_DAQ_CONFIG WHERE CONFIG_ID IN
    (SELECT DISTINCT CONFIG_ID FROM CMS_ECAL_CONF.RUN_FECONFIG_DAT D JOIN
 CMS_ECAL_R.RUN_IOV R ON D.IOV_ID = R.IOV_ID WHERE RUN_NUM = 344362);

 -> BTT_ID = 10488

  select  * from CMS_ECAL_CONF.fe_daq_bad_tt_dat where rec_id=10448;


     REC_ID	        TR_ID	 FED_ID      TT_ID	STATUS COMMENT
     10448          0        602          6          1
     10448          0        607          6          1
     10448          0        608         32          1
     10448          0        610         12          1
     10448          0        611         13          1
     10448          0        612          7          1
     10448          0        613          1          1
     10448          0        618         22          1
     10448          0        618         55          1
     10448          0        619         67          1
     10448          0        621          5          1
     10448          0        625         53          1
     10448          0        626         34          1
     10448          0        630         47          1
     10448          0        633          6          1
     10448          0        636         42          1 Masking tt 42 in fed eb+9
     10448          0        636         67          1
     10448          0        645         60          1
     10448          0        645         65          1
     10448          0        652         23          1 TT in EE+4
     10448          0        654         12          1
 */

using namespace dqmcpp;

namespace {

struct FEDTT {
  int fed;
  int tt;
};

static const std::array<FEDTT, 21> __maskedtt = {
    {{602, 6},  {607, 6},  {608, 32}, {610, 12}, {611, 13}, {612, 7},
     {613, 1},  {618, 22}, {618, 55}, {619, 67}, {621, 5},  {625, 53},
     {626, 34}, {630, 47}, {633, 6},  {636, 42}, {636, 67}, {645, 60},
     {645, 65}, {652, 23}, {654, 12}}};
}  // namespace

namespace dqmcpp {
namespace filters {

bool MaskedTT(const ECAL::TT& TT) {
  const int tower = TT.tt;
  const auto be = ECALChannels::list();
  const auto it = std::find_if(
      be.begin, be.end, [&TT](const ECALChannels::ChannelInfo& ci) {
        return ci.tcc == TT.tcc && ci.tower == TT.tt && ci.det_iz() == TT.iz;
      });
  if (it == be.end)
    return false;
  const int fed = it->fed;
  return MaskedTT(fed, tower);
};

bool MaskedTT(const int fed, const int tower) {
  return std::any_of(__maskedtt.begin(), __maskedtt.end(),
                     [fed, tower](const FEDTT& item) {
                       return item.fed == fed && item.tt == tower;
                     });
}

}  // namespace filters
}  // namespace dqmcpp