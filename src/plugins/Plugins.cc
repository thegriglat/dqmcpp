#include "Plugins.hh"

#include "RMSChannels.hh"
#include "TestPlugin.hh"
#include "TTF4Plugin.hh"

namespace Plugins
{

static const std::map<std::string, Plugin *> _pluginmap = {
    {"RMS", new RMSPlugin()},
    {"Test", new TestPlugin()},
    {"TTF4", new TTF4Plugin()}
};

Plugin *get(const std::string name)
{
    auto it = _pluginmap.find(name);
    if (it != _pluginmap.end())
        return it->second;
    std::cerr << "Plugin '" << name << "' not found!" << std::endl;
    return nullptr;
}

std::vector<std::string> list()
{
    std::vector<std::string> list;
    for (auto &e : _pluginmap) {
        list.push_back(e.first);
    }
    return list;
}
}