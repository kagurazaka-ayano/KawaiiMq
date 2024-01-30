# KawaiiMQ

A simple message queue implementation

Checkout [Examples](./doc/examples.md) for usage

## Build

Building tool: CMake

to build static library, run following command:
```bash
mkdir build
cd build
cmake -S .. -B .
cmake --build . --target KawaiiMQ
```

to run test, run
```bash
cmake --build .
ctest
```
in build directory

> Some tests are testing for timeout, which could take a long time to run, wait at least 20 seconds for each test.

to build documentation, run 
```bash
./scripts/makeDoc.sh
```

