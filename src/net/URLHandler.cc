/**
 * @file URLHandler.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief URLHander implementation
 */
#include "URLHandler.hh"
#include "../common/logging.hh"
#include "curl/curl.h"

namespace {
/**
 * @brief Helper for curl
 *
 * @param contents
 * @param size
 * @param nmemb
 * @param s
 * @return size_t
 */
size_t curl_write(void* contents, size_t size, size_t nmemb, std::string* s) {
  size_t newLength = size * nmemb;
  size_t oldLength = s->size();
  s->resize(oldLength + newLength);
  std::copy((char*)contents, (char*)contents + newLength,
            s->begin() + oldLength);
  return size * nmemb;
}

}  // namespace

namespace dqmcpp {
namespace net {

/**
 * @brief Returns content of URL
 *
 * @param url
 * @return std::string
 */
std::string URLHandler::get(const std::string& url) {
  std::string dbresponse;
  curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &dbresponse);
  auto res = curl_easy_perform(_curl);
  if (res != CURLE_OK)
    fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(res));
  return dbresponse;
}

URLHandler::URLHandler() {
  auto homedir_c = std::getenv("HOME");
  if (!homedir_c) {
    ERROR("Cannot get $HOME environment variable");
  }
  std::string homedir(homedir_c);
  pCertFile = homedir + "/.globus/usercert.pem";
  pKeyFile = homedir + "/.globus/userkey.pem";
  // Handle ENV vars
  char* envcert = nullptr;
  if ((envcert = std::getenv("USERCERT"))) {
    pCertFile = envcert;
  }
  if ((envcert = std::getenv("USERKEY"))) {
    pKeyFile = envcert;
  }
  _curl = curl_easy_init();
  if (!_curl) {
    ERROR("Cannot init curl!");
  } else {
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, curl_write);
    curl_easy_setopt(_curl, CURLOPT_SSL_VERIFYPEER, false);

    // certificates
    curl_easy_setopt(_curl, CURLOPT_SSLCERTTYPE, "PEM");
    curl_easy_setopt(_curl, CURLOPT_SSLCERT, pCertFile.c_str());
    curl_easy_setopt(_curl, CURLOPT_SSLKEY, pKeyFile.c_str());
    // TCP Keep Alive
    curl_easy_setopt(_curl, CURLOPT_TCP_KEEPALIVE, 1UL);
    curl_easy_setopt(_curl, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(_curl, CURLOPT_TCP_KEEPINTVL, 60L);
    // curl_easy_setopt(curl, CURLOPT_CAINFO, pCACertFile);
    curl_easy_setopt(_curl, CURLOPT_TCP_NODELAY, 1L);
    // max buffer size
    curl_easy_setopt(_curl, CURLOPT_BUFFERSIZE, CURL_MAX_READ_SIZE);
    // max cached connections
    curl_easy_setopt(_curl, CURLOPT_MAXCONNECTS, 10L);
    curl_easy_setopt(_curl, CURLOPT_ACCEPT_ENCODING,
                     "gzip,bzip2,deflate,br,zstd");
    curl_easy_setopt(_curl, CURLOPT_HTTPGET, 1L);
  }
}

}  // namespace net
}  // namespace dqmcpp