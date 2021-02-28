#include "URLCache.hh"
// #include "md5.hh"
#include <fstream>
#include <functional>

void URLCache::setCacheDir(const std::string &dir)
{
    _cachedir = dir;
}

std::string URLCache::hash(const std::string &text) const
{
    // return text;
    const auto hash = std::hash<std::string> {}(text);
    return std::to_string(hash);
}
std::string URLCache::cFileName(const std::string &url) const
{
    return _cachedir + "/" + hash(url);
}
std::string URLCache::getFromCache(const std::string &url) const
{
    // assuming that file exists
    std::ifstream in(cFileName(url));
    std::string content;
    in.seekg(0, std::ios::end);
    content.reserve(in.tellg());
    in.seekg(0, std::ios::beg);
    // read entire file
    content.assign((std::istreambuf_iterator<char>(in)),
                   std::istreambuf_iterator<char>());
    in.close();
    return content;
}

void URLCache::add(const std::string &url, const std::string &content)
{
    std::ofstream out(cFileName(url));
    out << content;
    out.close();
}

bool URLCache::has(const std::string &url) const
{
    std::ifstream in(cFileName(url));
    bool exists = in.good();
    in.close();
    return exists;
}

void URLCache::clean(void)
{
    // TODO: NOT implemented yet
    return;
}

std::string URLCache::get(const std::string &url)
{
    if (has(url)) {
        return getFromCache(url);
    }
    const auto content = _urlhandler->get(url);
    add(url, content);
    return content;
}