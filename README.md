# node-napi-cpp
Example on how to use Node.js N-API in C++

### Dependencies

- *node-cmake* - A CMake-based build system for Node.js native modules
- *node-addon-api* - This module contains header-only C++ wrapper classes which simplify the use of the C based Node-API provided by Node.js when using C++. It provides a C++ object model and exception handling semantics with low overhead
- *cmake-js* - CMake.js is a Node.js/io.js native addon build tool which works (almost) exactly like node-gyp, but instead of gyp, it is based on CMake build system.

Install dependencies before doing anything:

```sh
yarn add node-cmake node-addon-api cmake-js
```

### CMakeLists.txt

Tested on Node.js 12, 14 and 16, configuration made in a way that it usses `node-cmake` and `node-addon-api` bits installed earlier, so all your headers are in place and no additional configuration required. Well, just change your project name instead `demo` into something you like more at line #14.

