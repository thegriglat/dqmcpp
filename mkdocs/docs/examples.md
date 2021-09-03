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

Comments and explanations to code are inlined (mostly in `.cc` files). Please see file contents.


List of examples:

 * [Hello world](https://gitlab.cern.ch/ECALPFG/DQMCpp/-/tree/master/examples/hello_world)
    
    Simple "Hello world" plugin which does nothing. Just to see how to use namespaces, plugin registration and mandatory C++ code.

 * [Making HTTP requests](https://gitlab.cern.ch/ECALPFG/DQMCpp/-/tree/master/examples/requests)
    
    PLugin to learn how to get content by URL in different ways.