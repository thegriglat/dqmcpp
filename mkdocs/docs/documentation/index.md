# Documentation

## Common overview

DQMCpp is written to respect modularity. Each plugin is simular to standalone program, the difference is `Process(void)` function is called instead of `main(int, char**)`.

To read list of runs a plugin has access to `runListReader` private pointer:

    runListReader->runs() # will return std::vector<dqmcpp::ECAL::Run>

Reading, parsing and analysing data has to be done by plugin itself. There are no restrictions or helpers for that.

## Doxygen documentation

Prepare doxygen docs vith

    cd DQMCpp
    make docs
    # see 'doxygen' directory

## First plugin

Copy [hello_world example](https://github.com/thegriglat/dqmcpp/tree/main/examples/hello_world) to a new directory, run `cmake -DPLUGINDIR=<your example dir>` and build project.

Also check [examples](../examples.md) page.

You will be able to see your plugin in `dqm` help output

    dqm
    # or
    dqm -h
    # or
    dqm --help

!!! All plugins must be in `dqmcpp::plugins` namespace !!!

## Getting data from CMS DQM

See [here](dqm.md)

## Data structures

By default DQMCpp provides a few data structures for analysis. All of them can be found in [src/ECAL.hh](https://github.com/thegriglat/dqmcpp/blob/main/src/ECAL/ECAL.hh)

They are:

 * **Point1D** *for one-dimenstional histograms*
 * **Point2D** *for two-dimensional histograms*
 * **Channel** *for ECAL channels*
 * **TT** (trigger tower) *for ECAL TTs*
 * **CCU** *for ECAL CCUs*
 * **TTCCU** *to handle TT and CCU both in STL containers*
 * **Run** *to keep runnumber and dataset in one struct*
 * Templated types:
     * **Data<T\>** (templated key and `double` value)
     * **RunData<T\>** (Run with templated Data)

Here a list of some predefined types:

```C++
template <typename T>
struct Data {
  T base;
  double value;
};

template <typename T>
struct RunData {
  Run run;
  T data;
};

using ChannelData = Data<Channel>;
using TTData = Data<TT>;
using CCUData = Data<CCU>;
using TTCCUData = Data<TTCCU>;
using Data1D = Data<Point1D>;
using Data2D = Data<Point2D>;

using RunTTData = RunData<std::vector<TTData>>;
using RunCCUData = RunData<std::vector<CCUData>>;
using RunTTCCUData = RunData<std::vector<TTCCUData>>;
using RunChannelData = RunData<std::vector<ChannelData>>;
using RunData2D = RunData<std::vector<Data2D>>;
using RunData1D = RunData<std::vector<Data1D>>;
```
