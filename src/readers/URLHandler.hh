#ifndef URLHANDLER_HH

#include <string>
#include "curl/curl.h"

/**
 * @brief Helper class for curl operations
 * 
 */
class URLHandler
{
private:
    CURL *_curl = nullptr;

public:
    URLHandler();
    ~URLHandler()
    {
        curl_easy_cleanup(_curl);
    }
    /**
     * @brief Call operator to get URL content
     * 
     * @param url 
     * @return std::string 
     */
    std::string operator()(const std::string &url);
};

#define URLHANDLER_HH
#endif