/**
 * @file MaskedTT.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Masked TT filter from CMS_ECAL_CONF.fe_daq_bad_tt_dat table
 *
 */

#ifndef MASKEDTTFILTER_HH
#include "../ECAL/ECAL.hh"

namespace dqmcpp {
namespace filters {

/**
 * @brief Check if TT is masked in CMS_ECAL_CONF.fe_daq_bad_tt_dat table
 *
 * @param TT Trigger Tower object
 * @return true
 * @return false
 */
bool MaskedTT(const ECAL::TT& TT);
/**
 * @brief Check if TT is masked in CMS_ECAL_CONF.fe_daq_bad_tt_dat table
 *
 * @param fed FED #
 * @param tower Trigger Tower #
 * @return true
 * @return false
 */
bool MaskedTT(const int fed, const int tower);

}  // namespace filters
}  // namespace dqmcpp

#define MASKEDTTFILTER_HH
#endif