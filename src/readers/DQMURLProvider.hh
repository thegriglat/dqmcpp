#ifndef DQMURLPROVIDER_HH

#include <string>

namespace DQMURL
{
/**
 * @brief Return URL for json file based on parameters
 * Use "online" as dataset for Online DQM
 *
 * @param run
 * @param dataset
 * @param plotname
 * @return std::string
 */
std::string dqmurl(const unsigned int run, const std::string dataset, const std::string plotname);
}

#define DQMURLPROVIDER_HH
#endif