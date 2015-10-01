# Author: Björn Berg, bjoern.berg@gmx.de
# Date: 2015-07-27

# minimum version of cmake required to build this project
cmake_minimum_required(VERSION 2.6)

# The projects name and the language type.
# The C is important. Otherwise the indexer in Eclipse will have problems to resolve
# dependencies from some header files.
project(metro-spb C)
# Load our modules for this project
include_directories(/home/lolka/pebble-dev/PebbleSDK-3.4/Pebble/basalt/include)
file(GLOB SOURCE_FILES src/*.c)
add_executable(metro-spb ${SOURCE_FILES})
add_custom_target(pebble-build COMMAND)
