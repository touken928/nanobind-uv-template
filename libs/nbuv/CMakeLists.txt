# Standalone, Python-agnostic C++ core library.
#
# This target has no dependency on nanobind / Python; the bindings in
# packages/nbuv/bindings link to it and only translate calls to/from Python.
#
# Works two ways:
#   1. add_subdirectory(libs/nbuv) from a parent CMake project, or
#      via packages/nbuv/CMakeLists.txt (Python / scikit-build-core build).
#   2. As the CMake project root on its own:
#        cmake -S libs/nbuv -B build-nbuv

cmake_minimum_required(VERSION 3.15...3.30)

if(NOT DEFINED PROJECT_NAME)
  project(nbuv LANGUAGES CXX)
  # Standalone build: pull in the shared options and enable tests by default.
  set(NBUV_STANDALONE_LIB TRUE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  include(NBUVOptions)
endif()

option(NBUV_BUILD_TESTS
  "Build GoogleTest-based unit tests for the nbuv C++ library"
  ${NBUV_STANDALONE_LIB})

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

if(COMMAND nbuv_apply_warnings)
  nbuv_apply_warnings(nbuv)
endif()

if(NBUV_BUILD_TESTS)
  include(CTest)
  if(BUILD_TESTING)
    add_subdirectory(tests)
  endif()
endif()
