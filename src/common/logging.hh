#ifndef LOGGING_HH

/**
 * @file logging.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Logging/debugging macro
 * @todo Add -DNDEBUG handling
 */

#include <iostream>

#define ERROR(X)                                                               \
  (std::cerr << "ERROR: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" \
             << std::endl)
#define WARNING(X)                                                             \
  (std::cerr << "ERROR: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" \
             << std::endl)
#define INFO(X)                                                                \
  (std::cerr << "ERROR: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" \
             << std::endl)
#define DEBUG(X)                                                               \
  (std::cout << "DEBUG: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" \
             << std::endl)

#define LOGGING_HH
#endif