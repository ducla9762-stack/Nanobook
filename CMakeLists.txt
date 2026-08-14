cmake_minimum_required(VERSION 3.20)
project(nanobook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "" FORCE)
endif()

# Warnings are errors from the first commit. A matching engine that compiles with
# warnings is a matching engine with an uninitialised read waiting to happen.
add_library(nanobook_flags INTERFACE)
target_compile_options(nanobook_flags INTERFACE
  -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
  -Wshadow -Wold-style-cast -Wnon-virtual-dtor -Werror)

# The shared vocabulary: types, messages, events, reason codes. Header-only, and
# used by every engine implementation and every test.
add_library(nanobook_core INTERFACE)
target_include_directories(nanobook_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# The reference engine. Uncomment these two lines once src/reference_engine.cpp
# exists, and add nanobook_reference to the test target's link libraries.
# add_library(nanobook_reference STATIC src/reference_engine.cpp)
# target_link_libraries(nanobook_reference PUBLIC nanobook_core PRIVATE nanobook_flags)

option(NANOBOOK_BUILD_TESTS "Build the test suite" ON)
if(NANOBOOK_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
