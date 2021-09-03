# Installation

## Required packages

DQMCpp is C++ library, you need:

 * C++ compiler with at least C++11 support (GCC and Clang usually) 
 * [CMake](https://cmake.org/)
 * cURL library and headers installed
 * `make` utility
 * [Doxygen](https://www.doxygen.nl/index.html) to generate documentation


### Debian-like systems (Debian, Ubuntu)

    sudo apt-get install make g++ cmake libcurl4 libcurl4-openssl-dev

### Centos-like systems

    sudo yum install make g++ cmake libcurl4 libcurl4-openssl-devel

## Build from source

At the moment we don't provide built packages, so you have to build DQMCpp from source.

Here a copy-paste snippet to clone and build default DQMCpp libraries (with ECAL PFG plugins).

    git clone https://gitlab.cern.ch/ECALPFG/DQMCpp.git
    cd DQMCpp
    mkdir build
    cd build
    cmake ../src
    make # or make -j $(nproc)

To specify your own plugin diretory use cmake `-DPLUGINDIR` option.


## Build documentation

    cd DQMCpp
    make docs

Documentation will be written in `doxygen` directory.

# Configuration

Prepare certificate and decrypted key files

0. [Obtain GRID certificate](https://ca.cern.ch/ca/) if you don't have one.

1. Then you need to save your GRID certificate in p12 format. It is the same format as used by `voms-proxy-init`. Please follow instructions provided by CERN for [IE](https://ca.cern.ch/ca/Help/?kbid=031015), [Firefox](https://ca.cern.ch/ca/Help/?kbid=040111) or [Mac](https://ca.cern.ch/ca/Help/?kbid=060111).


2. Extract certificate part by executing

        openssl pkcs12 -in myCert.p12 -clcerts -nokeys -out usercert.pem

    (you will be asked for the password used when saving the certificate)

3. Extract private key by executing

        openssl pkcs12 -in myCert.p12 -nocerts -out userkey_encrypted.pem

    (you will be asked for the password used when saving the certificate, and then for a password to encrypt the key with).

4. Decrypt private key:

        openssl rsa -in userkey_encrypted.pem -out userkey.pem

    (you will be asked for the password from Step 3). You can now remove `userkey_encrypted.pem`

5. Change permissions for the decrypted key:

        chmod go-rw userkey.pem


After these steps copy `userkey.pem` and `usercert.pem` to `~/.globus` directory (default for CERN lxplus)

Configuration done!