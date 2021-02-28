#ifndef LOGGING_HH

#include <iostream>

#define ERROR(X) (std::cerr << "ERROR: " << (X) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl)

#define LOGGING_HH
#endif