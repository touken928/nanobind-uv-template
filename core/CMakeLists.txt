# Standalone, Python-agnostic C++ core library.
#
# This target has no dependency on nanobind / Python; the bindings in ../bindings
# link to it and only translate calls to/from Python.
#
# Works two ways:
#   1. `add_subdirectory(core)` from the top-level CMake project.
#   2. As a top-level project on its own (`cmake -S core -B build-core`)
#      for pure C++ development.
if(NOT DEFINED PROJECT_NAME)
  cmake_minimum_required(VERSION 3.15...3.30)
  project(nbuv_core LANGUAGES CXX)
endif()

add_library(nbuv_core STATIC
  src/math.cpp
  src/greeter.cpp
)
add_library(nbuv::core ALIAS nbuv_core)

target_include_directories(nbuv_core PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(nbuv_core PUBLIC cxx_std_17)

# The library is linked into a shared module, so compile it as PIC even when
# produced as a STATIC archive.
set_target_properties(nbuv_core PROPERTIES
  POSITION_INDEPENDENT_CODE ON
)
