<h1 align="center">nanobind-uv-template</h1>

<p align="center">
  <strong>Batteries-included GitHub template for a Python package whose core is C++: <a href="https://nanobind.readthedocs.io/">nanobind</a>, <a href="https://cmake.org/">CMake</a>, <a href="https://docs.astral.sh/uv/">uv</a> — <code>libs/</code> C++ libraries and <code>packages/</code> Python distributions in one <a href="https://docs.astral.sh/uv/concepts/workspaces/">uv workspace</a>.</strong>
</p>

<p align="center">
  <a href="https://www.python.org/downloads/"><img src="https://img.shields.io/badge/python-3.12-blue.svg?style=for-the-badge&logo=python" alt="Python 3.12"></a>
  <a href="https://en.cppreference.com/w/cpp/17"><img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++17"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/license-Apache%202.0-blue.svg?style=for-the-badge" alt="License: Apache 2.0"></a>
  <a href="https://pypi.org/project/nbuv/"><img src="https://img.shields.io/pypi/v/nbuv.svg?style=for-the-badge&logo=pypi&logoColor=white&label=pypi" alt="PyPI version"></a>
  <a href="https://github.com/touken928/nanobind-uv-template/stargazers"><img src="https://img.shields.io/github/stars/touken928/nanobind-uv-template?style=for-the-badge&color=yellow&logo=github" alt="GitHub stars"></a>
</p>

<p align="center"><a href="README_zh.md">简体中文</a></p>

The Python build backend is
[`scikit-build-core`](https://scikit-build-core.readthedocs.io/); `uv` drives
it transparently during `uv sync` / `uv build`.

Template repository:
<https://github.com/touken928/nanobind-uv-template>

> `nanobind-uv-template` is only the **GitHub template's name**. The example
> distribution is **`nbuv`**: package name on PyPI, import name, C++ namespace,
> and CMake library target (`nbuv::nbuv`) all share it. Rename `nbuv` to
> whatever you want after cloning.

## Use this template

1. Click **"Use this template" → "Create a new repository"** on GitHub, or
   `git clone https://github.com/touken928/nanobind-uv-template my-pkg`.
2. Rename the example project `nbuv` to your own name:

   ```bash
   # rename on disk
   git mv libs/nbuv libs/<your_pkg>
   git mv packages/nbuv packages/<your_pkg>
   git mv packages/<your_pkg>/src/nbuv packages/<your_pkg>/src/<your_pkg>

   # list every file that still references `nbuv`
   rg -l '\bnbuv\b'
   ```
3. Update `packages/<your_pkg>/pyproject.toml`: `[project].name`, `authors`,
   `urls`, `version`, etc.
4. Run `uv sync` and you're off.

## Layout

```
nanobind-uv-template/
├── pyproject.toml           # uv workspace root (virtual; no [project])
├── cmake/
│   └── nbuv/                # nbuv CMake modules (NBUVOptions, NBUVWarnings)
│
├── libs/
│   └── nbuv/                # (1) nbuv::nbuv — pure C++17 library, Python-agnostic
│       ├── CMakeLists.txt
│       ├── include/nbuv/
│       │   ├── math.hpp
│       │   └── greeter.hpp
│       ├── src/
│       │   ├── math.cpp
│       │   └── greeter.cpp
│       └── tests/           #     GoogleTest unit tests (FetchContent, ctest-ready)
│
└── packages/
    └── nbuv/                # (2) Python distribution `nbuv`
        ├── pyproject.toml   #     scikit-build-core backend + project metadata
        ├── CMakeLists.txt   #     add_subdirectory(libs/nbuv) + bindings
        ├── bindings/        #     nanobind glue — nbuv._core
        │   ├── CMakeLists.txt
        │   └── _core.cpp
        ├── src/nbuv/        #     Python package (must match wheel.packages path tail)
        │   ├── __init__.py
        │   └── py.typed
        └── tests/           #     pytest
            └── test_basic.py
```

Dependencies flow one way: `libs/nbuv` ← `packages/*/bindings` ← `packages/*/src`.

- **`libs/nbuv/`** holds all business logic in plain C++17. It has no knowledge
  of Python and can be reused by any other CMake project via `add_subdirectory`
  (target `nbuv::nbuv`).
- **`packages/nbuv/bindings/`** is intentionally thin: `#include` the
  `nbuv/*.hpp` headers and call `m.def(...)` / `nb::class_<...>(...)` — no logic.
- **`packages/nbuv/src/nbuv/`** holds the importable package: `__init__.py`
  re-exports `_core`. `scikit-build-core` expects the wheel package folder name
  (last path segment in `[tool.scikit-build.wheel.packages]`) to match the
  distribution / import name — here `nbuv = src/nbuv`.

## Requirements

- C++17 compiler (Clang 8+ / GCC 8+ / MSVC 2019+)
- CMake 3.15+ (fetched automatically by `scikit-build-core` if missing)
- [uv](https://docs.astral.sh/uv/getting-started/installation/)

## Quick start — Python

```bash
# 1. Create the venv, compile the extension, install in editable mode.
uv sync

# 2. Try it out.
uv run python -c "import nbuv; print(nbuv.add(2, 3), nbuv.Greeter('uv').greet())"

# 3. Run the Python test suite.
uv run pytest packages/nbuv/tests

# 4. Build the nbuv wheel (and/or sdist) into ./dist/.
uv build --package nbuv --wheel
uv build --package nbuv --sdist
```

Thanks to `[tool.uv] cache-keys` in `packages/nbuv/pyproject.toml`, changes
under `libs/nbuv/**` / `bindings/**` / `cmake/**` automatically trigger a
rebuild on the next `uv sync` / `uv run`.

The wheel is tagged `cp312-abi3` (Python stable ABI). Package metadata sets
`requires-python == 3.12.*` in `packages/nbuv/pyproject.toml` — edit that field
if you need a different Python range.

## Quick start — C++ only

Use `libs/nbuv/CMakeLists.txt` as the C++ entry point (library + GoogleTest):

```bash
cmake -S libs/nbuv -B build-nbuv
cmake --build build-nbuv -j
ctest --test-dir build-nbuv --output-on-failure
```

Build options (append to the `cmake -S libs/nbuv ...` line, e.g.
`-DNBUV_BUILD_TESTS=OFF`):

- `NBUV_BUILD_TESTS` — build C++ unit tests under `libs/*/tests` (default `ON`).

To consume the library from another CMake project:

```cmake
add_subdirectory(path/to/nanobind-uv-template/libs/nbuv)
target_link_libraries(my_app PRIVATE nbuv::nbuv)
```

## Releasing

Pushing a `v*` tag runs two workflows in parallel:

| Workflow | What it does |
| -------- | -------------- |
| `.github/workflows/release.yml` | Builds wheels + sdist and attaches them to a GitHub Release. |
| `.github/workflows/pypi.yml` | Builds wheels + sdist and uploads them to **PyPI** (`pip install nbuv`). |

Configure [PyPI Trusted Publishing](https://docs.pypi.org/trusted-publishers/)
once (GitHub workflow `pypi.yml`, environment `pypi`). On tag push, the PyPI
job is skipped if the tag name contains `-` (treated as a pre-release). Use a
[repository secret](https://docs.github.com/en/actions/security-guides/using-secrets-in-github-actions)
`PYPI_API_TOKEN` instead if you do not use Trusted Publishing — see the
comments in `pypi.yml`.

```bash
uv version X.Y.Z             # bump version in packages/nbuv/pyproject.toml
git commit -am "Release vX.Y.Z"
git tag vX.Y.Z
git push && git push --tags
```

**From PyPI** (after a successful upload):

```bash
pip install nbuv
```

**From GitHub Releases** — use a direct asset URL (copy the exact `.whl` name
from the release; it embeds the version and platform tags):

```bash
pip install https://github.com/touken928/nanobind-uv-template/releases/latest/download/<wheel-file>
```

After each release, wheel filenames change with the version; check
[Releases](https://github.com/touken928/nanobind-uv-template/releases/latest)
if you install by URL.

> **Source builds from the published sdist are not supported** — `libs/nbuv`
> and `cmake/` live outside `packages/nbuv/`, so the sdist tarball is
> metadata-only. Build from `git clone` or install a prebuilt wheel.

## Extending the template

- **Add C++ functionality**: put headers in `libs/nbuv/include/nbuv/` and
  sources in `libs/nbuv/src/`, append them to `add_library(nbuv ...)`
  in `libs/nbuv/CMakeLists.txt`, and add a `test_*.cpp` under
  `libs/nbuv/tests/`.
- **Expose it to Python**: add `m.def(...)` / `nb::class_<...>(...)` calls in
  `packages/nbuv/bindings/_core.cpp`. The `from ._core import *` in
  `__init__.py` picks them up automatically.
- **Add pure-Python helpers**: drop them in
  `packages/nbuv/src/nbuv/__init__.py` or new submodules beside it.
- **Add another C++ library**: `mkdir libs/<name>` following `libs/nbuv/`, then
  `add_subdirectory` it from your own top-level CMake project (or vendor it the
  same way `packages/nbuv/CMakeLists.txt` pulls in `libs/nbuv`).
- **Add a C++ dependency**: `find_package` / `FetchContent` in
  `libs/nbuv/CMakeLists.txt`, then
  `target_link_libraries(nbuv PRIVATE ...)`. The binding layer inherits
  it transitively.
- **Add a Python dependency**: `uv add <pkg>` from the workspace root; mark
  dev-only with `uv add --dev <pkg>`.

## References

- [nanobind documentation](https://nanobind.readthedocs.io/en/latest/)
- [scikit-build-core documentation](https://scikit-build-core.readthedocs.io/)
- [uv documentation](https://docs.astral.sh/uv/)
- [uv workspaces](https://docs.astral.sh/uv/concepts/workspaces/)

## License

Licensed under the [Apache License, Version 2.0](LICENSE). See that file for the
full text.
