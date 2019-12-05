
# mbed-rpc

mbed-rpc is an [RPC](https://en.wikipedia.org/wiki/Remote_procedure_call) library based on [Embedded RPC](https://github.com/EmbeddedRPC/erpc) originally created by Freescale/NXP.

It is very lightweight (a few kB of flash and RAM, depending on buffer sizes) and flexible. It allows simplified inter-processor communication in multicore chips or multi-chip boards over a variety of transports. erpc also provides an abstract intermediate language to specify RPC interfaces -- allowing implementations to be generated for multiple languages (currently, C/C++ and Python are supported).

This parent library handles porting it to Mbed and its build system outside of erpc's source.

## Getting Started

It is assumed at this point that you have read erpc overview documentation and have some familiarity with erpc concepts. If you aren't familiar with basic usage of erpc, go and read its documentation first.

### Examples

Examples of using this library can be found in this associated repository:

https://github.com/EmbeddedPlanet/ep-oc-example-mbed-rpc

### Installing erpc tools

To get started with mbed-rpc, first install the erpc tools that are used to convert interface definition language (IDL) files to the client/server "shim" code. 

**Note:** You must first install erpc's dependencies on your system. These include bison, flex, and boost. You can install these by running the following:

```
sudo apt update
sudo apt install bison
sudo apt install flex
sudo apt-get install libboost-all-dev
```

Then, to install the erpc tools, execute the following:

```
cd erpc
sudo make install erpc erpcgen
sudo make clean
```

This will build and install the erpc library and erpcgen tool for your system. You must clean up the build directories to make sure Mbed's build system doesn't try to pull them in! The erpc library is required to use the Python binding. You can install the python binding by executing:

```
cd erpc/erpc_python
python setup.py install
```

## Library Configuration

The `mbed_lib.json` file that is a part of this library simply configures parameters exposed by erpc. In most cases, you will not have to edit the configuration to use the mbed-rpc library.

You can override the default configuration using Mbed's JSON-based configuration system.

### Baremetal (RTOS-less) Build
One case where it may be beneficial to change the config is in a baremetal build with no Mbed RTOS. In this case, you should set the `erpc-threads` parameter to 0 to disable multithreading. To avoid build errors, you may also need to ignore the following files in the erpc source:

- erpc/erpc_c/infra/erpc_arbitrated_client_manager.cpp
- erpc/erpc_c/infra/erpc_arbitrated_client_manager.h
- erpc/erpc_c/infra/erpc_transport_arbitrator.cpp
- erpc/erpc_c/infra/erpc_transport_arbitrator.h

These files use threading and may cause the build to fail if not ignored.

**Note:** If building with no RTOS, a combined RPC client/server application will not be possible using the built-in erpc classes. They use multithreading constructs to share access to the same transport between the client and server code.
