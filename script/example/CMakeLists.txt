cmake_minimum_required (VERSION 3.1.1)

project(EXAMPLE CXX)

# Gathering source files
file(GLOB_RECURSE source_files ../../source/*)
file(GLOB_RECURSE script_files ../../script/example/*)

# Include
include_directories(../../include ../../source ../../script/example)

# Finding DACE
find_package(dace REQUIRED)

# Main executable
add_executable(example ${script_files}  ${source_files} ../../script/example/example.cpp)

# Link
target_link_libraries(example PUBLIC dace::dace_s)
