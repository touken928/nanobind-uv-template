# Standalone, Python-agnostic C++ core library.
#
# This target has no dependency on nanobind / Python; the bindings at the
# repository root link to it and only translate calls to/from Python.
#
# Works two ways:
#   1. add_subdirectory(libs/nbuv) from a parent CMake project, or
#      via the repository root CMakeLists.txt (Python / scikit-build-core build).
#   2. As the CMake project root on its own:
#        cmake -S libs/nbuv -B build-nbuv

cmake_minimum_required(VERSION 3.15...3.30)

# Global defaults for this repo (also applied before bindings/ when loaded from root).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "")
if(NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON CACHE BOOL "")
endif()

function(nbuv_apply_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE
      -Wall -Wextra -Wpedantic -Wshadow)
  endif()
endfunction()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  project(nbuv LANGUAGES CXX)
endif()

add_library(nbuv STATIC
  src/math.cpp
  src/greeter.cpp
)
add_library(nbuv::nbuv ALIAS nbuv)

target_include_directories(nbuv PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(nbuv PUBLIC cxx_std_17)

set_target_properties(nbuv PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)

nbuv_apply_warnings(nbuv)
