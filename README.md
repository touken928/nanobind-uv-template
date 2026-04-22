<h1 align="center">nanobind-uv-template</h1>

<p align="center">
  <strong>GitHub template: C++ core + Python via <a href="https://nanobind.readthedocs.io/">nanobind</a>, <a href="https://cmake.org/">CMake</a>, <a href="https://docs.astral.sh/uv/">uv</a>. Pure C++ lives under <code>libs/</code>; the sample distribution <code>nbuv</code> is at the repo root.</strong>
</p>

<p align="center">
  <a href="https://www.python.org/downloads/"><img src="https://img.shields.io/badge/python-3.12-blue.svg?style=for-the-badge&logo=python" alt="Python 3.12"></a>
  <a href="https://en.cppreference.com/w/cpp/17"><img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++17"></a>
  <a href="https://github.com/touken928/nanobind-uv-template/actions/workflows/ci.yml"><img src="https://img.shields.io/github/actions/workflow/status/touken928/nanobind-uv-template/ci.yml?branch=main&logo=github&label=CI&style=for-the-badge" alt="CI"></a>
</p>

<p align="center"><a href="README_zh.md">简体中文</a></p>

Build backend: [`scikit-build-core`](https://scikit-build-core.readthedocs.io/). Template repo: <https://github.com/touken928/nanobind-uv-template>.

The example package **`nbuv`** is used everywhere (PyPI name, import, CMake target `nbuv::nbuv`). Rename it after cloning.

## Use this template

1. **Use this template** on GitHub or `git clone … my-pkg`.
2. Rename directories and fix references:

   ```bash
   git mv libs/nbuv libs/<your_pkg>
   git mv src/nbuv src/<your_pkg>
   rg -l '\bnbuv\b'
   ```

3. Edit root `pyproject.toml` (`[project]`, `[tool.scikit-build.wheel.packages]`, URLs, version) and CMake / C++ names.
4. `uv sync`.

## Layout

```
nanobind-uv-template/
├── pyproject.toml
├── CMakeLists.txt
├── bindings/           # nanobind → nbuv._core
├── src/nbuv/
├── tests/
└── libs/nbuv/          # nbuv::nbuv (C++ only)
    ├── CMakeLists.txt
    ├── include/nbuv/
    └── src/
```

Flow: `libs/nbuv` ← `bindings/` ← `src/nbuv`. The wheel package path tail must match the distribution name (`nbuv = src/nbuv`).

## Prerequisites

C++17, CMake 3.15+, [uv](https://docs.astral.sh/uv/getting-started/installation/).

## Python

```bash
uv sync
uv run pytest tests
uv build          # ./dist/
```

Changing `libs/nbuv/**` or `bindings/**` triggers a rebuild on the next `uv sync` (`[tool.uv]` cache-keys). Stable ABI wheel targets Python 3.12 (`requires-python` in `pyproject.toml`).

## C++ only

```bash
cmake -S libs/nbuv -B build-nbuv
cmake --build build-nbuv -j
```

```cmake
add_subdirectory(path/to/.../libs/nbuv)
target_link_libraries(my_app PRIVATE nbuv::nbuv)
```

## CI and releases

- **PR / push to `main`:** [`.github/workflows/ci.yml`](.github/workflows/ci.yml) — `pytest` on Ubuntu and `macos-15-intel`.
- **Tag `v*`:** [`.github/workflows/release.yml`](.github/workflows/release.yml) (GitHub Release artifacts) and [`.github/workflows/pypi.yml`](.github/workflows/pypi.yml) (PyPI). Configure [Trusted Publishing](https://docs.pypi.org/trusted-publishers/) for `pypi.yml` / environment `pypi`, or use secret `PYPI_API_TOKEN` (see `pypi.yml`). Tags containing `-` skip the PyPI job (pre-release).

```bash
uv version X.Y.Z
git commit -am "Release vX.Y.Z"
git tag vX.Y.Z && git push && git push --tags
```

Install: `pip install nbuv` from PyPI, or download wheels from [Releases](https://github.com/touken928/nanobind-uv-template/releases/latest).

## Extending

- C++: add sources under `libs/nbuv/`, list them in `add_library(nbuv …)`; test via `tests/`.
- Python surface: `bindings/_core.cpp` + `src/nbuv/`.
- Dependencies: `target_link_libraries(nbuv …)` / `uv add`.

## Docs

[nanobind](https://nanobind.readthedocs.io/en/latest/) · [scikit-build-core](https://scikit-build-core.readthedocs.io/) · [uv](https://docs.astral.sh/uv/)

## License

Example distribution **nbuv**: [Apache-2.0](LICENSE).
