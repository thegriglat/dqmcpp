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

Comments and explanations to code are inlined (mostly in `plugin.cc` files).


List of examples:

 * [Hello world](https://github.com/thegriglat/dqmcpp/tree/main/examples/hello_world)
    
    Simple "Hello world" plugin which does nothing. Just to see how to use namespaces, plugin registration and write mandatory C++ code.

 * [Making HTTP requests](https://github.com/thegriglat/dqmcpp/tree/main/examples/requests)
    
    Plugin to learn how to get content by URL in different ways.

 * [Channel database](https://github.com/thegriglat/dqmcpp/tree/main/examples/channelsdb)

    Plugin to learn how to operate with ECAL channel database.

 * [Parallel processing](https://github.com/thegriglat/dqmcpp/tree/main/examples/parallel)

    Plugin to learn how to easily run your functions in parallel.

To be added ...

Of course the best examples are [real PFG plugins](https://github.com/thegriglat/dqmcpp/tree/main/PFGplugins)