#
# CMake Configuration for making node.js native addons.
# Tested On Node.js 12+
# @author skitsanos, https://github.com/skitsanos
# @version 1.0.2
#
# To use, run yarn or npm install first, so it will add dependencies as well
#
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # not necessary, but encouraged

project(demo CXX)

# Include node headers
include(./node_modules/node-cmake/NodeJS.cmake)
nodejs_init()

# After init, we will have headers required at the following folder:
# ${CMAKE_CURRENT_BINARY_DIR}/node/${NODEJS_VERSION}/include

include_directories(${CMAKE_JS_INC} "${CMAKE_CURRENT_BINARY_DIR}/node/${NODEJS_VERSION}/include" "${CMAKE_HOME_DIRECTORY}/node_modules/node-addon-api")

file(GLOB SOURCE_FILES
        "./src/*.cpp"
        "./src/*.h")

# This line will tell CMake that we're building a shared library
# from the above source files
# named after the project's name
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# This line will give our library file a .node extension
# without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential library files to link to a node addon,
# you should add this line in every CMake.js based project.
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})