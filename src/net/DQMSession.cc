/**
 * @file DQMSession.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief DQM session manager's code
 */
#include "DQMSession.hh"
#include "URLHandler.hh"

namespace {
std::string sessionUrl(const bool offline) {
  if (offline)
    return "https://cmsweb.cern.ch/dqm/offline/session/";
  return "https://cmsweb.cern.ch/dqm/online/session/";
}

// local string cache
std::string currentSession = "";

}  // namespace

namespace dqmcpp {
namespace net {
namespace DQMSession {

std::string get(const bool offline) {
  /*
1. GET https://cmsweb.cern.ch/dqm/offline/session/
2. Parse output for session id
3. Return it.
*/

  // return current session if it is set
  if (currentSession.size() != 0) {
    // get() called multiple times at one program call
    return currentSession;
  }
  net::URLHandler urlhdlr;
  auto content = urlhdlr.get(sessionUrl(offline));

  // Returns smth like:
  // <html><head><script>location.replace('/dqm/offline/session/ABMTY3')</script></head><body><noscript>Please
  // enable JavaScript to use this service</noscript></body></html>

  // strip everything except session ID
  const std::string session_pattern = "session/";
  auto sessionpos = content.find(session_pattern);
  content.erase(0, sessionpos + session_pattern.length());
  auto endpos = content.find("'");
  content.erase(endpos);

  currentSession = content;
  return content;
}

}  // namespace DQMSession
}  // namespace net
}  // namespace dqmcpp