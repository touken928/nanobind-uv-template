cmake_minimum_required(VERSION 3.15...3.30)

project(nbuv
  LANGUAGES CXX
  DESCRIPTION "nanobind bindings + CMake + uv template")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# This top-level file wires the C++ core library to the Python binding layer.
# It is driven by scikit-build-core during `uv sync` / `uv build` / `pip install`.
#
# Pure C++ users who don't need Python can build the core library directly via:
#     cmake -S core -B build-core
# or integrate it into their own CMake project with:
#     add_subdirectory(path/to/nbuv/core)

add_subdirectory(core)

find_package(Python 3.12
  REQUIRED COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT)
find_package(nanobind CONFIG REQUIRED)

add_subdirectory(bindings)
