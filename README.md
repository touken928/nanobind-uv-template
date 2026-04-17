# nanobind-uv-template

[English](README.md) | [中文](README_zh.md)

A minimal, batteries-included GitHub template for shipping a Python package
whose core is written in C++. It wires up three moving parts:

- [**nanobind**](https://nanobind.readthedocs.io/) — fast, small C++/Python bindings
- [**CMake**](https://cmake.org/) — the native build system
- [**uv**](https://docs.astral.sh/uv/) — the Python environment / packaging frontend

The Python build backend is
[`scikit-build-core`](https://scikit-build-core.readthedocs.io/); `uv` drives
it transparently during `uv sync` / `uv build`.

Template repository:
<https://github.com/touken928/nanobind-uv-template>

> `nanobind-uv-template` is only the **GitHub template's name**. Everything
> inside the repo — the Python distribution, the import name, the C++
> namespace, the library target, the CMake project — is called **`nbuv`**.
> Rename `nbuv` to whatever you want after cloning.

## Use this template

1. Click **"Use this template" → "Create a new repository"** on GitHub, or
   `git clone https://github.com/touken928/nanobind-uv-template my-pkg`.
2. Rename the example project `nbuv` to your own name. `nbuv` only appears in
   these places:

   ```bash
   # rename on disk
   git mv src/nbuv src/<your_pkg>

   # list every file that still references `nbuv`
   rg -l '\bnbuv\b'        # pyproject.toml, CMakeLists.txt, core/**, bindings/**,
                           # src/<your_pkg>/__init__.py, tests/*, README*
   ```
3. Update `pyproject.toml`: `[project].name` (currently `nbuv`), `authors`,
   `urls`, `version`, etc.
4. Run `uv sync` and you're off.

## Layout

```
nanobind-uv-template/
├── CMakeLists.txt        # top-level, wires core/ + bindings/
├── pyproject.toml        # scikit-build-core backend + project metadata
│
├── core/                 # (1) standalone C++ library — nbuv::core
│   ├── CMakeLists.txt    #     pure C++, no Python / nanobind dependency
│   ├── include/nbuv/
│   │   ├── math.hpp
│   │   └── greeter.hpp
│   └── src/
│       ├── math.cpp
│       └── greeter.cpp
│
├── bindings/             # (2) nanobind glue — nbuv._core
│   ├── CMakeLists.txt    #     nanobind_add_module + link nbuv::core
│   └── _core.cpp         #     argument / return-value conversion only
│
├── src/nbuv/             # (3) Python package facade
│   ├── __init__.py       #     `from ._core import *` — add Python code here
│   └── py.typed
│
└── tests/
    └── test_basic.py
```

Dependencies flow one way: `core` ← `bindings` ← `src/nbuv`.

- **`core/`** holds all business logic in plain C++17. It has no knowledge of
  Python and can be reused by any other CMake project via `add_subdirectory`.
- **`bindings/`** is intentionally thin. It only `#include`s the `nbuv/*.hpp`
  headers and calls `m.def(...)` / `nb::class_<...>(...)` — no logic.
- **`src/nbuv/__init__.py`** re-exports the compiled `_core` submodule as the
  package's public API. This is also where you drop any pure-Python helpers.

## Requirements

- Python **3.12+**
- C++17 compiler (Clang 8+ / GCC 8+ / MSVC 2019+)
- CMake 3.15+ (fetched automatically by `scikit-build-core` if missing)
- [uv](https://docs.astral.sh/uv/getting-started/installation/)

## Quick start

```bash
# 1. Create the venv, compile the extension, install in editable mode.
uv sync

# 2. Try it out.
uv run python -c "import nbuv; print(nbuv.add(2, 3), nbuv.Greeter('uv').greet())"

# 3. Run the test suite.
uv run --group dev pytest

# 4. Build distributable artifacts (wheel + sdist) into dist/.
uv build
```

Thanks to `[tool.uv] cache-keys` in `pyproject.toml`, changes under
`core/**` / `bindings/**` / `CMakeLists.txt` automatically trigger a rebuild
on the next `uv sync` / `uv run`.

The wheel is tagged `cp312-abi3` (Python stable ABI), so a single artifact
works across Python 3.12 and every later 3.x release.

## Releasing

Pushing a tag that starts with `v` triggers `.github/workflows/release.yml`,
which cross-builds wheels on Linux / Windows / macOS, builds an sdist on
Linux, and publishes them as assets on a GitHub Release with the same tag
name.

```bash
uv version X.Y.Z             # bump version in pyproject.toml
git commit -am "Release vX.Y.Z"
git tag vX.Y.Z
git push && git push --tags
```

Install the published wheels with a direct asset URL (copy the exact
`.whl` name from the release — it embeds the package version and platform
tags):

```bash
pip install https://github.com/touken928/nanobind-uv-template/releases/latest/download/<wheel-file>
```

After you publish a new version, asset names change with the version segment;
open [Releases](https://github.com/touken928/nanobind-uv-template/releases/latest)
and refresh the URL you use.

> GitHub Packages does not currently offer a Python (PyPI) registry.
> **GitHub Releases** is the idiomatic way to ship pre-built wheels from a
> GitHub repository, which is what this workflow uses.

## Using `core/` as a plain C++ library

`core/` is self-contained and has no dependency on Python or nanobind.

```bash
# Build it standalone (e.g. for C++ unit tests or CI without Python).
cmake -S core -B build-core && cmake --build build-core -j
```

```cmake
# Or pull it into any other CMake project.
add_subdirectory(path/to/nanobind-uv-template/core)
target_link_libraries(my_app PRIVATE nbuv::core)
```

## Extending the template

- **Add C++ functionality**: put headers in `core/include/nbuv/` and sources
  in `core/src/`, then append them to `add_library(nbuv_core ...)` in
  `core/CMakeLists.txt`.
- **Expose it to Python**: add `m.def(...)` / `nb::class_<...>(...)` calls in
  `bindings/_core.cpp`. The `from ._core import *` in `__init__.py` picks
  them up automatically.
- **Add pure-Python helpers**: drop them in `src/nbuv/__init__.py` or new
  submodules beside it.
- **Add a C++ dependency**: `find_package` / `FetchContent` in
  `core/CMakeLists.txt`, then `target_link_libraries(nbuv_core PRIVATE ...)`.
  The binding layer inherits it transitively.
- **Add a Python dependency**: `uv add <pkg>`; dev-only with
  `uv add --group dev <pkg>`.

## References

- [nanobind documentation](https://nanobind.readthedocs.io/en/latest/)
- [scikit-build-core documentation](https://scikit-build-core.readthedocs.io/)
- [uv documentation](https://docs.astral.sh/uv/)

## License

Licensed under the [Apache License, Version 2.0](LICENSE). See that file for the
full text.
