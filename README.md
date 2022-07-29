# ISAAC - ISA AutoTuned Compiler
ISAAC is a ISA level auto-tuning compiler. 

# Getting started

First, run the configure script and then build the project as shown below.

```
./configure.sh
cmake --build build
```

# Running tests

Run the GTest suite of tests using the following command.

```
ctest --test-dir build/tools/compiler
```
