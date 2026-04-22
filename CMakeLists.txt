# Python build entry point (driven by scikit-build-core).
#
# Scope: pull in the pure C++ library (libs/nbuv) and compile the nanobind
# glue layer (bindings/) into the `_core` extension module. Exercise the
# library via pytest under tests/. Standalone C++ builds use libs/nbuv/CMakeLists.txt.

cmake_minimum_required(VERSION 3.15...3.30)

project(nbuv
  LANGUAGES CXX
  DESCRIPTION "nanobind bindings + CMake + uv template")

# Bring in the pure-C++ library from the repo's libs/ directory (also applies C++
# defaults before bindings/).
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/nbuv
  ${CMAKE_CURRENT_BINARY_DIR}/libs-nbuv
)

# nanobind discovery.
find_package(Python 3.12
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
find_package(nanobind CONFIG REQUIRED)

add_subdirectory(bindings)
