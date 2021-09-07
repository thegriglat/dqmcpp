# Working with CMS DQM

DQMCpp uses cURL library to request JSON (really JSON-like) data from CMS DQM.

Also to avoid DDoS of DQM we populate own cache with DQM data (by default in `$TMP` directory). The cache directory can be overridden by `CACHEDIR` environment variable.

By default all HTTP requests are cached and called sync. Non-caching request are also possible.

We provide relative low level functions to make request as well as high level parsers for DQM JSON for ECAL **channels**, **trigger towers**, **CCU** and custom 1/2-D histograms.

For parsing JSON [nlohmann json library](https://github.com/nlohmann/json) is used.

# DQM URL

Usually url is obtained from CMS DQM online or offline web pages by hand. Url consists of from `runnumber`, `dataset` and plot-specific path.

To construct url with given EB/EE number use `dqmcpp::common::string_format()` (it's like `printf` on steroids):

    std::string formatted_url = dqmcpp::common::string_format(
        "EcalEndcap/EEPedestalOnlineTask/Gain12/EEPOT pedestal EE%+03d G12",
        +5);
    
where `+5` is SM (super module) number.

Then pass this `formatted_url` to `dqmcpp::net::DQMURL::dqmurl()` to get DQM url for given `runnumber` and `dataset`:

    std::string dqm_url = dqmcpp::net::DQMURL::dqmurl(runnumber, dataset, formatted_url)

Also using `dqmcpp::ECAL::Run` object is possible

    auto run = dqmcpp::ECAL::Run(123456, "online");
    std::string dqm_url = dqmcpp::net::DQMURL::dqmurl(run, 
                          dqmcpp::common::string_format(
                            "EcalEndcap/EEPedestalOnlineTask/Gain12/EEPOT pedestal EE%+03d G12",
                          +5);)

# HTTP requests

** ALL REQUESTS ARE SYNCHRONOUS **

cURL library is used to process requests.

## Non-cached requests

To process request you have to use `URLHandler` object

    #include <string>
    #include "URLHandler.hh"

    dqmcpp::net::URLHandler urlhdlr; // will be automatically and properly closed and deleted after quitting scope
    std::string url = "http://foo.bar";
    std::string content = urlhdlr.get(url); // make synchronous request

## Cached requests

To process cached request you have to use `URLCache` object
    
    #include <string>
    #include "net/URLCache.hh"

    std::string url = "http://foo.bar";
    std::string content = dqmcpp::net::URLCache::get(url);

It is possible to request multiple data at one call
    
    #include <vector>
    #include <string>
    #include "net/URLCache.hh"

    std::vector<std::string> urls = {"http://foo.bar", "http://my.super.site"};
    std::vector<std::string> contents = dqmcpp::net::URLCache::get(urls);

# Parsing JSON data

to be continued ....
