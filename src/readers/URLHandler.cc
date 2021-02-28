
#include "URLHandler.hh"
#include "../logging/logging.hh"
#include "curl/curl.h"

/**
 * @brief Helper for curl
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param s
 * @return size_t
 */
static size_t curl_write(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size * nmemb;
    size_t oldLength = s->size();
    try {
        s->resize(oldLength + newLength);
    } catch (std::bad_alloc &e) {
        //handle memory problem
        return 0;
    }

    std::copy((char *)contents, (char *)contents + newLength, s->begin() + oldLength);
    return size * nmemb;
}

/**
 * @brief Returns content of URL
 *
 * @param url
 * @return std::string
 */
std::string URLHandler::operator()(const std::string &url)
{
    std::string dbresponse;
    curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &dbresponse);
    curl_easy_perform(_curl);
    return dbresponse;
}

URLHandler::URLHandler()
{
    _curl = curl_easy_init();
    if (!_curl) {
        ERROR("Cannot init curl!");
    } else {
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, curl_write);
        curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, false);
    }
}
