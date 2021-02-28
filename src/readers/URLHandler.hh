#ifndef URLHANDLER_HH

#include <string>
#include "curl/curl.h"
#include "../logging/logging.hh"

/**
 * @brief Helper class for curl operations
 *
 */
class URLHandler
{
private:
    CURL *_curl = nullptr;
    std::string pCertFile;
    std::string pKeyFile;

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
    std::string get(const std::string &url);
    inline void setUserCertFile(const std::string &certFile)
    {
        pCertFile = certFile;
    }
    inline void setUserKeyFile(const std::string &keyFile)
    {
        pKeyFile = keyFile;
    }
};

#define URLHANDLER_HH
#endif