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
cmake --build .
```

to run test, run
```bash
cmake . -DKawaiiMQ_BUILD_TESTS=ON
cmake --build .
ctest
```
in build directory

to build documentation, run 
```bash
./scripts/makeDoc.sh
```

