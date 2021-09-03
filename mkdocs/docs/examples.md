# List of examples

To compile and run examples follow [installation](installation.md) instruction.

To compile:

    cd DQMCpp
    mkdir build
    cd build
    cmake -DPLUGINDIR=../examples ../src
    make -j nproc

To run:

    touch dummy ## as dqm utility requires file with runs
    # examples itself doesn't require any runs (if not specially mentioned)
    ./dqm dummy <example plugin name>
    # for example ./dqm dummy hello_world

Comments and explanations to code are inlined (mostly in `plugin.cc` files). Please see file contents.


List of examples:

 * [Hello world](https://gitlab.cern.ch/ECALPFG/DQMCpp/-/tree/master/examples/hello_world)
    
    Simple "Hello world" plugin which does nothing. Just to see how to use namespaces, plugin registration and write mandatory C++ code.

 * [Making HTTP requests](https://gitlab.cern.ch/ECALPFG/DQMCpp/-/tree/master/examples/requests)
    
    Plugin to learn how to get content by URL in different ways.

 * [Channel database](https://gitlab.cern.ch/ECALPFG/DQMCpp/-/tree/master/examples/channelsdb)

    Plugin to learn how to operate with ECAL channel database.

 * [Parallel processing](https://gitlab.cern.ch/ECALPFG/DQMCpp/-/tree/master/examples/parallel)

    Plugin to learn how to easily run your functions in parallel.

To be added ...

Of course the best examples are [real plugins](https://gitlab.cern.ch/ECALPFG/DQMCpp/-/tree/master/PFGplugins)