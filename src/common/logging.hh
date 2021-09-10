#ifndef LOGGING_HH

/**
 * @file logging.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Macroses
 *
 *
 */
#include <iostream>

/**
 * @brief ERROR macro
 *
 */
#define ERROR(X)                                                               \
  (std::cerr << "ERROR: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" \
             << std::endl)

/**
 * @brief WARNING macro
 *
 */
#define WARNING(X)                                                             \
  (std::cerr << "ERROR: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" \
             << std::endl)

/**
 * @brief INFO macro
 *
 */
#define INFO(X)                                                                \
  (std::cerr << "ERROR: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" \
             << std::endl)

/**
 * @brief DEBUG macro
 *
 */
#define DEBUG(X)                                                               \
  (std::cout << "DEBUG: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" \
             << std::endl)

#define LOGGING_HH
#endif