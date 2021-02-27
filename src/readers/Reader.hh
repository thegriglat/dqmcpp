#ifndef READER_HH
/**
 * @brief Pure abstract class for readers
 * 
 */

#include "URLCache.hh"

class Reader
{
    Reader(URLCache *cache = nullptr);
};

#define READER_HH
#endif