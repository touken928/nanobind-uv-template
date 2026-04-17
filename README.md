<h1 align="center">nanobind-uv-template</h1>

<p align="center">
  <strong>Batteries-included GitHub template for a Python package whose core is C++: <a href="https://nanobind.readthedocs.io/">nanobind</a>, <a href="https://cmake.org/">CMake</a>, and <a href="https://docs.astral.sh/uv/">uv</a> — separate <code>core/</code> and <code>bindings/</code>; the sample project is <code>nbuv</code>.</strong>
</p>

<p align="center">
  <a href="https://www.python.org/downloads/"><img src="https://img.shields.io/badge/python-3.12+-blue.svg?style=for-the-badge&logo=python" alt="Python 3.12+"></a>
  <a href="https://en.cppreference.com/w/cpp/17"><img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++17"></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/license-Apache%202.0-blue.svg?style=for-the-badge" alt="License: Apache 2.0"></a>
  <a href="https://pypi.org/project/nbuv/"><img src="https://img.shields.io/pypi/v/nbuv.svg?style=for-the-badge&logo=pypi&logoColor=white&label=pypi" alt="PyPI version"></a>
  <a href="https://github.com/touken928/nanobind-uv-template/stargazers"><img src="https://img.shields.io/github/stars/touken928/nanobind-uv-template?style=for-the-badge&color=yellow&logo=github" alt="GitHub stars"></a>
</p>

<p align="center"><a href="README_zh.md">简体中文</a></p>

The Python build backend is[`scikit-build-core`](https://scikit-build-core.readthedocs.io/); `uv` drives it transparently during `uv sync` / `uv build`.

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

Pushing a `v*` tag runs two workflows in parallel:

| Workflow | What it does |
| -------- | -------------- |
| `.github/workflows/release.yml` | Builds wheels + sdist and attaches them to a GitHub Release. |
| `.github/workflows/pypi.yml` | Builds wheels + sdist and uploads them to **PyPI** (`pip install nbuv`). |

Configure [PyPI Trusted Publishing](https://docs.pypi.org/trusted-publishers/) once (GitHub workflow `pypi.yml`, environment `pypi`). On tag push, the PyPI job is skipped if the tag name contains `-` (treated as a pre-release). Use a [repository secret](https://docs.github.com/en/actions/security-guides/using-secrets-in-github-actions) `PYPI_API_TOKEN` instead if you do not use Trusted Publishing—see the comments in `pypi.yml`.

```bash
uv version X.Y.Z             # bump version in pyproject.toml
git commit -am "Release vX.Y.Z"
git tag vX.Y.Z
git push && git push --tags
```

**From PyPI** (after a successful upload):

```bash
pip install nbuv
```

**From GitHub Releases** — use a direct asset URL (copy the exact `.whl` name from the release; it embeds the version and platform tags):

```bash
pip install https://github.com/touken928/nanobind-uv-template/releases/latest/download/<wheel-file>
```

After each release, wheel filenames change with the version; check
[Releases](https://github.com/touken928/nanobind-uv-template/releases/latest)
if you install by URL.

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
