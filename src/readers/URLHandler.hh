#ifndef URLHANDLER_HH
/**
 * @file URLHandler.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Helper class for cURL
 */
#include <string>
#include "curl/curl.h"

class URLHandler {
 private:
  CURL* _curl = nullptr;
  std::string pCertFile;
  std::string pKeyFile;

 public:
  URLHandler();
  ~URLHandler() { curl_easy_cleanup(_curl); }
  /**
   * @brief Call operator to get URL content
   *
   * @param url URL
   * @return std::string
   */
  std::string get(const std::string& url);
  /**
   * @brief Set the User Cert File
   *
   * @param certFile Path to user cert file (usually ~/.globus/usercert.pem)
   */
  inline void setUserCertFile(const std::string& certFile) {
    pCertFile = certFile;
  }
  /**
   * @brief Set the User Key File
   *
   * @param keyFile Path to user key file (usually ~/.globus/userkey.pem)
   */
  inline void setUserKeyFile(const std::string& keyFile) { pKeyFile = keyFile; }
};

#define URLHANDLER_HH
#endif