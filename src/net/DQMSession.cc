/**
 * @file DQMSession.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief DQM session manager's code
 */
#include "DQMSession.hh"
#include "URLHandler.hh"

namespace {
// local string cache
std::string currentSession = "";
}  // namespace

namespace dqmcpp {
namespace net {
namespace DQMSession {

std::string get() {
  /*
1. GET https://cmsweb.cern.ch/dqm/offline/session/
2. Parse output for session id
3. Return it.

We should not use URLCache to be sure that new session returns every time
*/
  // return current session if it is set
  if (currentSession.length() != 0)
    return currentSession;
  static dqmcpp::net::URLHandler urlhdlr;
  static const std::string url = "https://cmsweb.cern.ch/dqm/offline/session/";
  auto content = urlhdlr.get(url);

  // Returns smth like:
  // <html><head><script>location.replace('/dqm/offline/session/ABMTY3')</script></head><body><noscript>Please
  // enable JavaScript to use this service</noscript></body></html>

  // strip everything except session ID
  const std::string session_pattern = "session/";
  auto sessionpos = content.find(session_pattern);
  content.erase(0, sessionpos + session_pattern.length());
  auto endpos = content.find("'");
  content.erase(endpos);

  // cache session
  currentSession = content;
  return content;
}

}  // namespace DQMSession
}  // namespace net
}  // namespace dqmcpp