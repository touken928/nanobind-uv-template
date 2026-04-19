<h1 align="center">nanobind-uv-template</h1>

<p align="center">
  <strong>开箱即用的 GitHub 模板，用于发布「核心在 C++」的 Python 包：<a href="https://nanobind.readthedocs.io/">nanobind</a>、<a href="https://cmake.org/">CMake</a>、<a href="https://docs.astral.sh/uv/">uv</a> —— <code>libs/</code> 放纯 C++ 库，Python 发行（示例 <code>nbuv</code>）与仓库根目录的 <code>pyproject.toml</code>、<code>bindings/</code>、<code>src/</code> 同级。</strong>
</p>

<p align="center">
  <a href="https://www.python.org/downloads/"><img src="https://img.shields.io/badge/python-3.12-blue.svg?style=for-the-badge&logo=python" alt="Python 3.12"></a>
  <a href="https://en.cppreference.com/w/cpp/17"><img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++17"></a>
  <a href="https://github.com/touken928/nanobind-uv-template/actions/workflows/ci.yml"><img src="https://img.shields.io/github/actions/workflow/status/touken928/nanobind-uv-template/ci.yml?branch=main&logo=github&label=CI&style=for-the-badge" alt="CI"></a>
  <a href="https://github.com/touken928/nanobind-uv-template/stargazers"><img src="https://img.shields.io/github/stars/touken928/nanobind-uv-template?style=for-the-badge&color=yellow&logo=github" alt="GitHub stars"></a>
</p>

<p align="center"><a href="README.md">English</a></p>

Python 侧构建后端是
[`scikit-build-core`](https://scikit-build-core.readthedocs.io/)，`uv sync` /
`uv build` 时由 `uv` 透明驱动。

模板仓库地址：
<https://github.com/touken928/nanobind-uv-template>

> `nanobind-uv-template` **只是这份 GitHub 模板的名字**。示例分发叫
> **`nbuv`**：PyPI 包名、`import` 模块名、C++ 命名空间、库 target
> (`nbuv::nbuv`) 全部共用这个名字。clone 之后把 `nbuv` 全局替换成你自己
> 的项目名即可。

## 如何使用本模板

1. 在 GitHub 上点 **"Use this template" → "Create a new repository"**，
   或者直接
   `git clone https://github.com/touken928/nanobind-uv-template my-pkg`。
2. 把示例项目 `nbuv` 改成你自己的名字：

   ```bash
   # 重命名目录
   git mv libs/nbuv libs/<your_pkg>
   git mv src/nbuv src/<your_pkg>

   # 列出所有还写着 nbuv 的文件
   rg -l '\bnbuv\b'
   ```
3. 更新仓库根目录 `pyproject.toml` 里的 `[project].name`、
   `[tool.scikit-build.wheel.packages]`、`authors`、`urls`、`version`，并同步
   CMake / C++ 命名空间等。
4. `uv sync` 就可以开工了。

## 目录布局

```
nanobind-uv-template/
├── pyproject.toml           # scikit-build-core + `[project]`（nbuv）
├── CMakeLists.txt           # add_subdirectory(libs/nbuv) + bindings
├── bindings/                # nanobind 绑定 —— nbuv._core
│   ├── CMakeLists.txt
│   └── _core.cpp
├── src/nbuv/                # Python 包（须与 wheel.packages 路径末段一致）
│   ├── __init__.py
│   └── py.typed
├── tests/                   # pytest
│   └── test_basic.py
│
└── libs/
    └── nbuv/                # nbuv::nbuv —— 纯 C++17，不依赖 Python
        ├── cmake/           # NBUVOptions、NBUVWarnings（CMake 预设）
        ├── CMakeLists.txt
        ├── include/nbuv/
        │   ├── math.hpp
        │   └── greeter.hpp
        ├── src/
        │   ├── math.cpp
        │   └── greeter.cpp
        └── tests/           # GoogleTest 单测（FetchContent + ctest）
```

依赖方向是单调的：`libs/nbuv` ← `bindings/` ← `src/nbuv/`。

- **`libs/nbuv/`** 存放所有业务逻辑，纯 C++17。完全不知道 Python 的存在，任何
  CMake 项目都能通过 `add_subdirectory` 把它拿来用（target `nbuv::nbuv`）。
- **`bindings/`** 刻意做得很薄：只 `#include "nbuv/*.hpp"` 然后
  `m.def(...)` / `nb::class_<...>(...)`，不放业务逻辑。
- **`src/nbuv/`** 为可 import 的包目录；`__init__.py` 转发 `_core`。
  `scikit-build-core` 要求 `[tool.scikit-build.wheel.packages]` 里路径的**最后一级
  目录名**与包名一致，因此使用 `nbuv = src/nbuv`。

## 环境要求

- C++17 编译器（Clang 8+ / GCC 8+ / MSVC 2019+）
- CMake 3.15+（`scikit-build-core` 会在需要时自动拉取）
- [uv](https://docs.astral.sh/uv/getting-started/installation/)

## 快速开始 —— Python

```bash
# 1. 创建虚拟环境，编译扩展，可编辑安装。
uv sync

# 2. 试一下。
uv run python -c "import nbuv; print(nbuv.add(2, 3), nbuv.Greeter('uv').greet())"

# 3. 跑 Python 测试套件。
uv run pytest tests

# 4. 将 wheel 与 sdist 输出到 ./dist/（`uv build` 默认会打两种包）。
uv build
```

根目录 `pyproject.toml` 中 `[tool.uv] cache-keys` 已配置好，修改
`libs/nbuv/**` / `bindings/**`（含 `libs/nbuv/**/*.cmake`）之后再次 `uv sync` / `uv run`
会自动重新编译。

wheel 使用 `cp312-abi3`（Python 稳定 ABI）标签。根目录 `pyproject.toml`
里 `requires-python` 目前为 `==3.12.*`；若需支持其他 Python 版本请自行修改。

## 快速开始 —— 只编 C++

C++ 入口是 `libs/nbuv/CMakeLists.txt`（静态库 + GoogleTest）：

```bash
cmake -S libs/nbuv -B build-nbuv
cmake --build build-nbuv -j
ctest --test-dir build-nbuv --output-on-failure
```

构建选项（追加到 `cmake -S libs/nbuv ...` 后面，如 `-DNBUV_BUILD_TESTS=OFF`）：

- `NBUV_BUILD_TESTS` —— 构建 `libs/*/tests` 下的 C++ 单测（默认 `ON`）。

从别的 CMake 项目里引用：

```cmake
add_subdirectory(path/to/nanobind-uv-template/libs/nbuv)
target_link_libraries(my_app PRIVATE nbuv::nbuv)
```

## 发版

对 `main` 的 **push** 与 **pull request** 会跑
[`.github/workflows/ci.yml`](.github/workflows/ci.yml)（`pytest` + C++ 测试）。
推送 `v*` tag 还会**并行**跑下面两个发布工作流：

| 工作流 | 作用 |
| ------ | ---- |
| `.github/workflows/ci.yml` | 在 `main` 上跑 Python 与 C++ 测试。 |
| `.github/workflows/release.yml` | 构建 wheel + sdist，并挂到 GitHub Release。 |
| `.github/workflows/pypi.yml` | 构建 wheel + sdist，并上传到 **PyPI**（`pip install nbuv`）。 |

在 PyPI 上配置一次
[Trusted Publishing](https://docs.pypi.org/trusted-publishers/)
（关联本仓库的 workflow `pypi.yml`、environment `pypi`）。若 **push** 的 tag
名称里含 `-`（视为预发布），会跳过 PyPI 上传。若不用 Trusted Publishing，
可用仓库密钥 `PYPI_API_TOKEN`——见 `pypi.yml` 内注释。

```bash
uv version X.Y.Z             # 修改 pyproject.toml 里的版本号
git commit -am "Release vX.Y.Z"
git tag vX.Y.Z
git push && git push --tags
```

**从 PyPI 安装**（上传成功后）：

```bash
pip install nbuv
```

**从 GitHub Releases 安装**——使用资源直链（从 Release 页面复制完整 `.whl`
文件名，其中含版本与平台标签）：

```bash
pip install https://github.com/touken928/nanobind-uv-template/releases/latest/download/<wheel-file>
```

每次发版后 wheel 文件名会随版本变化；若用 URL 安装，请到
[Releases](https://github.com/touken928/nanobind-uv-template/releases/latest)
核对。

> 日常使用请优先装 **wheel**（`pip install nbuv`）。也会上传 **sdist**；从 sdist 安装会在本地编译扩展，需要 C++ 工具链、CMake、Ninja 等。

## 在模板上扩展

- **加 C++ 功能**：把头文件写到 `libs/nbuv/include/nbuv/`、源文件写到
  `libs/nbuv/src/`，然后追加到 `libs/nbuv/CMakeLists.txt` 的
  `add_library(nbuv ...)`；在 `libs/nbuv/tests/` 加一个对应的
  `test_*.cpp`。
- **暴露到 Python**：在 `bindings/_core.cpp` 里用
  `m.def(...)` / `nb::class_<...>(...)`。`__init__.py` 里的
  `from ._core import *` 会自动带上。
- **加纯 Python 工具函数**：直接写进
  `src/nbuv/__init__.py` 或并列加新子模块。
- **加另一个 C++ 库**：`mkdir libs/<name>`，照 `libs/nbuv/` 的结构写；在你自
  己的顶层 CMake 里 `add_subdirectory`，或像仓库根目录 `CMakeLists.txt`
  引用 `libs/nbuv` 那样嵌进去。
- **加 C++ 依赖**：在 `libs/nbuv/CMakeLists.txt` 里 `find_package` 或
  `FetchContent`，再 `target_link_libraries(nbuv PRIVATE ...)`；
  绑定层会自动继承。
- **加 Python 依赖**：在仓库根目录执行 `uv add <pkg>` / `uv add --dev <pkg>`
  （更新 `pyproject.toml` 与 `[dependency-groups].dev`）。

## 参考

- [nanobind 文档](https://nanobind.readthedocs.io/en/latest/)
- [scikit-build-core 文档](https://scikit-build-core.readthedocs.io/)
- [uv 文档](https://docs.astral.sh/uv/)

## 许可证

本仓库中的 **`nbuv`** 分发采用 [Apache License, Version 2.0](LICENSE)。
