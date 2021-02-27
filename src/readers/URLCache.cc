#include "URLCache.hh"

bool URLCache::has(const std::string &url) const
{
    return false;
}

void URLCache::clean(void)
{
    return;
}

using json = nlohmann::json;
nlohmann::json get(const std::string &url)
{
    json dummy;
    return dummy;
}