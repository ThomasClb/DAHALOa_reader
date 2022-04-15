cmake_minimum_required (VERSION 3.1.1)

project(EXAMPLE CXX)

# Gathering source files
file(GLOB_RECURSE source_files ../../source/*)
file(GLOB_RECURSE script_files ../../script/example/*)

# Include

# Librairies
include_directories(../../../lib/dace/interfaces/cxx/include)
include_directories(../../../lib/dace/build/core/include)
include_directories(../../../lib/dace/core/include)

# Local files
include_directories(../../include)
include_directories(../../source)
include_directories(../../script/example)

# Linking librairies
link_directories(../../../lib/dace/build/interfaces/cxx)

# Main executable
add_executable(example ${script_files}  ${source_files} ../../script/example/run_example.cpp)

# Properties
set_target_properties(example PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED TRUE
    CXX_EXTENSIONS FALSE)

# Link
target_link_libraries(
    example 
    dace)
