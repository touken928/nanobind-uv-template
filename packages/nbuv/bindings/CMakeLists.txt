# nanobind glue: turns the pure-C++ nbuv::nbuv library into the `nbuv._core`
# Python submodule. The `nbuv` Python package (src/nbuv/__init__.py) then
# re-exports its symbols as the public API.

nanobind_add_module(
  _core
  STABLE_ABI
  NB_STATIC
  _core.cpp
)

target_link_libraries(_core PRIVATE nbuv::nbuv)

install(TARGETS _core LIBRARY DESTINATION nbuv)

# PEP 561 stub for IDE / type-checker support, shipped next to the extension.
nanobind_add_stub(
  _core_stub
  MODULE _core
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/_core.pyi
  PYTHON_PATH $<TARGET_FILE_DIR:_core>
  DEPENDS _core
)
install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/_core.pyi
  DESTINATION nbuv
)
