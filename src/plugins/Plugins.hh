#ifndef PLUGINFACTORY_HH
/**
 * @file Plugins.hh
 * @author Grigory Latyshev (thegriglat@gmail.com)
 * @brief Namespace to get Plugin* by name
 */
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "../readers/RunListReader.hh"

namespace dqmcpp {
namespace plugins {

/* Base class for plugins */
class Plugin {
 protected:
  dqmcpp::readers::RunListReader* runListReader = nullptr;

 public:
  /**
   * @brief Destroy the Plugin object
   *
   */
  virtual ~Plugin(){};
  /**
   * @brief Main function for Plugin
   *
   */
  virtual void Process(void) = 0;

  /**
   * @brief Set the Run List Reader object
   *
   * @param runlistreader RunListReader* pointer
   */
  inline void setRunListReader(dqmcpp::readers::RunListReader* runlistreader) {
    runListReader = runlistreader;
  }
};

/*
 * Base class for PluginRegistrar
 * See PluginRegistrar below for explanations
 */
class IPluginRegistrar {
 public:
  virtual std::unique_ptr<Plugin> GetPlugin() = 0;
};

/*
 * This is the factory, the common interface to "plugins".
 * Plugins registers themselves here and the factory can serve them on
 * demand.
 * It is a Singleton
 */
class PluginFactory {
 public:
  /* Get Singleton instance */
  static PluginFactory& Instance();
  /* Register a new plugin */
  void Register(IPluginRegistrar* registrar, std::string name);
  /* Get an instance of a plugin based on its name */
  /* throws out_of_range if plugin not found */
  std::unique_ptr<Plugin> GetPlugin(std::string name);
  inline std::vector<std::string> list() const {
    std::vector<std::string> l;
    for (auto& pair : registry_) {
      l.push_back(pair.first);
    }
    return l;
  }

 private:
  /* Holds pointers to plugin registrars */
  std::map<std::string, IPluginRegistrar*> registry_;
  /* Make constructors private and forbid cloning */
  PluginFactory() : registry_(){};
  PluginFactory(PluginFactory const&) = delete;
  void operator=(PluginFactory const&) = delete;
};

/*
 * Helper class that registers a plugin upon construction.
 * Actually, the registrar registers itself, and the proxied plugin is only
 * created on-demand. This mechanism can be shortened by directly
 * registering and instance of the plugin, but the assumption here is that
 * instanciating the plugin can be heavy and not necessary.
 */
template <class TPlugin>
class PluginRegistrar : public IPluginRegistrar {
 public:
  PluginRegistrar(std::string classname);
  std::unique_ptr<Plugin> GetPlugin();

 private:
  /* That is not really used there, but could be useful */
  std::string classname_;
};

/* template functions in header */

template <class TPlugin>
PluginRegistrar<TPlugin>::PluginRegistrar(std::string classname)
    : classname_(classname) {
  PluginFactory& factory = PluginFactory::Instance();
  factory.Register(this, classname);
}

template <class TPlugin>
std::unique_ptr<Plugin> PluginRegistrar<TPlugin>::GetPlugin() {
  std::unique_ptr<Plugin> plugin(new TPlugin());
  return plugin;
}

}  // namespace plugins
}  // namespace dqmcpp

/*
 * Here is the trick: upon creation of the global variable, the class created
 * out of the template will get instanciated once, and will register itself.
 * The template contains the information to create a plugin instance.
 * An unnamed namespace is used to enclose this later unused variable in the
 * compilation unit.
 */
#define REGISTER_PLUGIN0(CLASSNAME, PLUGINNAME)                               \
  namespace {                                                                 \
  using namespace dqmcpp::plugins;                                            \
  static dqmcpp::plugins::PluginRegistrar<CLASSNAME> _registrar(#PLUGINNAME); \
  };

#define REGISTER_PLUGIN1(CLASSNAME) REGISTER_PLUGIN0(CLASSNAME, CLASSNAME)

#define GET_REGISTRATION_MACRO(_1, _2, NAME, ...) NAME

/**
 * @brief Register plugin macro
 * use
 *  REGISTER_PLUGIN (classname, pluginname)
 * or
 *  REGISTER_PLUGIN(classname)
 *
 */
#define REGISTER_PLUGIN(...)                                              \
  GET_REGISTRATION_MACRO(__VA_ARGS__, REGISTER_PLUGIN0, REGISTER_PLUGIN1) \
  (__VA_ARGS__)

#define PLUGINFACTORY_HH
#endif