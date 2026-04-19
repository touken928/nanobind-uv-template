# GoogleTest-based unit tests for the pure C++ nbuv library.
# Pulled in via FetchContent so no system-wide GoogleTest install is required.

include(FetchContent)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
  URL_HASH SHA256=7b42b4d6ed48810c5362c265a17faebe90dc2373c885e5216439d37927f02926
)
FetchContent_MakeAvailable(googletest)

add_executable(nbuv_tests
  test_math.cpp
  test_greeter.cpp
)

target_link_libraries(nbuv_tests PRIVATE
  nbuv::nbuv
  GTest::gtest_main
)

if(COMMAND nbuv_apply_warnings)
  nbuv_apply_warnings(nbuv_tests)
endif()

include(GoogleTest)
gtest_discover_tests(nbuv_tests)
