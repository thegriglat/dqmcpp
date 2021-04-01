/**
 * @file DQMSession.cc
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief DQM session manager's code
 */
#include "DQMSession.hh"
#include <fstream>
#include "URLCache.hh"
#include "URLHandler.hh"

#define TMPFILE ".dqmsession"

namespace {
const std::string SESSIONURL = "https://cmsweb.cern.ch/dqm/offline/session/";

// local string cache
std::string currentSession = "";

bool sessionExpired(const std::string& session) {
  if (session.size() == 0)
    return true;
  dqmcpp::net::URLHandler hdlr;
  auto content = hdlr.get(SESSIONURL + session);
  /*
   If expired smth like
   ===
   The page you accessed, /dqm/offline/session/123123412345, is not a valid page
   or you may have tried to access content that belongs to someone else that was
   not made "public."
   ===
   will be returned
   Check for "is not a valid page"
  */
  return (content.find("is not a valid page") != content.npos);
}

std::string getCachedSession() {
  std::ifstream fh(TMPFILE);
  std::string session;
  fh >> session;
  fh.close();
  return session;
}

void setCachedSession(const std::string& session) {
  std::ofstream fh(TMPFILE);
  fh << session;
  fh.close();
}
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

  if (currentSession.size() != 0) {
    // get() called multiple times at one program call
    return currentSession;
  }
  /**
   * @todo Why we do at least one GET request in any case?
   * int sessionExpired() and in current fn
   */
  {
    auto session = getCachedSession();
    if (!sessionExpired(session)) {
      // not expired
      currentSession = session;
      return session;
    }
  }
  // otherwise -- get new session
  net::URLHandler urlhdlr;
  auto content = urlhdlr.get(SESSIONURL);

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
  setCachedSession(currentSession);
  return content;
}

}  // namespace DQMSession
}  // namespace net
}  // namespace dqmcpp