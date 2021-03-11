#ifndef DQMURLPROVIDER_HH
/**
 * @file DQMURLProvider.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Provide simple functions for DQM urls
 */
#include <string>
#include <vector>
#include "../ECAL/ECAL.hh"

namespace dqmcpp {
namespace net {
namespace DQMURL {

/**
 * @brief Return URL for json file based on parameters
 * Use "online" as dataset for Online DQM
 *
 * @param run Run number
 * @param dataset CMS Dataset or "online" for Online DQM
 * @param plotname Plot path in DQM terms. See examples in plugins/ directory in
 * url() method.
 * @return std::string
 */
std::string dqmurl(const unsigned int run,
                   const std::string dataset,
                   const std::string plotname);
/**
 * @brief Get list of datasets for given run and mask
 *
 * @param run Run
 * @param mask Dataset mask, wildcards allowed
 * @param useLast if True will return only last version of simular datasets
 * @return std::vector<std::string>
 */
std::vector<dqmcpp::ECAL::Run> runs(const unsigned int run,
                                    const std::string mask,
                                    const bool useLast = true);

}  // namespace DQMURL
}  // namespace net
}  // namespace dqmcpp
#define DQMURLPROVIDER_HH
#endif