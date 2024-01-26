# message queue

A simple message queue implementation

## Build

Building tool: CMake

to build static library, run following command:
```bash
mkdir build
cd build
cmake -S .. -B .
cmake --build . --target KawaiiMQ
```

to build test, run following command:
```bash
mkdir build
cd build
cmake -S .. -B .
cmake --build . --target KawaiiMQ_TEST
```

to build documentation, run `./scripts/makeDoc.sh`

