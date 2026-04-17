# nanobind-uv-template

[English](README.md) | [中文](README_zh.md)

一个极简且开箱即用的 GitHub 模板项目，用来发布一个核心用 C++ 写的 Python 包。
它把三样东西串起来：

- [**nanobind**](https://nanobind.readthedocs.io/) — 轻量、高效的 C++/Python 绑定
- [**CMake**](https://cmake.org/) — C++ 构建系统
- [**uv**](https://docs.astral.sh/uv/) — Python 环境与打包前端

Python 侧构建后端是
[`scikit-build-core`](https://scikit-build-core.readthedocs.io/)，
`uv sync` / `uv build` 时由 `uv` 透明驱动。

模板仓库地址：
<https://github.com/touken928/nanobind-uv-template>

> **命名约定**
>
> `nanobind-uv-template` **只是这份 GitHub 模板的名字**。仓库内部的一切——
> Python 分发名、`import` 模块名、C++ 命名空间、库 target、CMake project——
> 统一都叫 **`nbuv`**。clone 之后，把 `nbuv` 全局替换成你自己的项目名即可。

## 如何使用本模板

1. 在 GitHub 上点 **"Use this template" → "Create a new repository"**，
   或者直接
   `git clone https://github.com/touken928/nanobind-uv-template my-pkg`。
2. 把示例项目 `nbuv` 改成你自己的名字。`nbuv` 这个字符串只出现在下面这些地方：

   ```bash
   # 重命名目录
   git mv src/nbuv src/<your_pkg>

   # 列出所有还写着 nbuv 的文件
   rg -l '\bnbuv\b'     # pyproject.toml、CMakeLists.txt、core/**、bindings/**、
                        # src/<your_pkg>/__init__.py、tests/*、README*
   ```
3. 更新 `pyproject.toml` 里的 `[project].name`（目前是 `nbuv`）、`authors`、
   `urls`、`version` 等。
4. `uv sync` 就可以开工了。

## 目录布局

```
nanobind-uv-template/
├── CMakeLists.txt        # 顶层，串起 core/ 与 bindings/
├── pyproject.toml        # scikit-build-core 后端 + 项目元数据
│
├── core/                 # (1) 独立 C++ 库 —— nbuv::core
│   ├── CMakeLists.txt    #     纯 C++，不依赖 Python / nanobind
│   ├── include/nbuv/
│   │   ├── math.hpp
│   │   └── greeter.hpp
│   └── src/
│       ├── math.cpp
│       └── greeter.cpp
│
├── bindings/             # (2) nanobind 绑定 —— nbuv._core
│   ├── CMakeLists.txt    #     nanobind_add_module + 链接 nbuv::core
│   └── _core.cpp         #     只做参数 / 返回值翻译，无业务逻辑
│
├── src/nbuv/             # (3) Python 包门面
│   ├── __init__.py       #     `from ._core import *` —— 加 Python 代码写这里
│   └── py.typed
│
└── tests/
    └── test_basic.py
```

依赖方向是单调的：`core` ← `bindings` ← `src/nbuv`。

- **`core/`** 存放所有业务逻辑，纯 C++17。完全不知道 Python 的存在，任何
  CMake 项目都能通过 `add_subdirectory` 把它拿来用。
- **`bindings/`** 刻意做得很薄——只 `#include "nbuv/*.hpp"` 然后
  `m.def(...)` / `nb::class_<...>(...)`，不放业务逻辑。
- **`src/nbuv/__init__.py`** 把编译好的 `_core` 子模块转发成包的公开 API；
  想加纯 Python 的工具函数，也是写在这里。

## 环境要求

- Python **3.12+**
- C++17 编译器（Clang 8+ / GCC 8+ / MSVC 2019+）
- CMake 3.15+（`scikit-build-core` 会在需要时自动拉取）
- [uv](https://docs.astral.sh/uv/getting-started/installation/)

## 快速开始

```bash
# 1. 创建虚拟环境，编译扩展，可编辑安装。
uv sync

# 2. 试一下。
uv run python -c "import nbuv; print(nbuv.add(2, 3), nbuv.Greeter('uv').greet())"

# 3. 跑测试。
uv run --group dev pytest

# 4. 打包 wheel + sdist 到 dist/。
uv build
```

`pyproject.toml` 中 `[tool.uv] cache-keys` 已配置好，修改
`core/**` / `bindings/**` / `CMakeLists.txt` 之后再次 `uv sync` / `uv run`
会自动重新编译。

因使用 Python 稳定 ABI（`STABLE_ABI`），wheel 带 `cp312-abi3` 标签——
一份产物即可在 Python 3.12 及以后所有 3.x 版本上复用。

## 发版

推一个 `v` 开头的 tag 就会触发 `.github/workflows/release.yml`——它会在
Linux / Windows / macOS 上各编一份 wheel，在 Linux 上再做一份 sdist，
然后把它们作为 assets 挂到同名的 GitHub Release 上。

```bash
uv version 0.1.0             # 修改 pyproject.toml 里的版本号
git commit -am "Release v0.1.0"
git tag v0.1.0
git push && git push --tags
```

消费这些 wheel：

```bash
# 直接指定某个 wheel 的下载链接
pip install https://github.com/touken928/nanobind-uv-template/releases/download/v0.1.0/<wheel-file>

# 或者把 release 目录当成索引，让 pip/uv 自动选当前平台的 wheel
pip install \
  --index-url https://github.com/touken928/nanobind-uv-template/releases/download/v0.1.0/ \
  nbuv
uv add \
  --index https://github.com/touken928/nanobind-uv-template/releases/download/v0.1.0/ \
  nbuv
```

> GitHub Packages **目前没有** Python (PyPI) 注册源。把 wheel 挂到
> **GitHub Releases** 是在 GitHub 上分发 wheel 的通行做法，本工作流就是
> 这么做的。

## 把 `core/` 当普通 C++ 库用

`core/` 自给自足，不依赖 Python / nanobind：

```bash
# 独立构建（比如写 C++ 单测、在没 Python 的 CI 上 sanity check）
cmake -S core -B build-core && cmake --build build-core -j
```

```cmake
# 或从别的 CMake 项目里直接用
add_subdirectory(path/to/nanobind-uv-template/core)
target_link_libraries(my_app PRIVATE nbuv::core)
```

## 扩展这个模板

- **加 C++ 功能**：头文件丢到 `core/include/nbuv/`，源文件丢到 `core/src/`，
  然后在 `core/CMakeLists.txt` 的 `add_library(nbuv_core ...)` 里补上。
- **暴露到 Python**：在 `bindings/_core.cpp` 里用
  `m.def(...)` / `nb::class_<...>(...)` 把符号包一层；
  `__init__.py` 里的 `from ._core import *` 会自动带上。
- **加纯 Python 工具函数**：直接写进 `src/nbuv/__init__.py` 或并列加新子模块。
- **加 C++ 第三方依赖**：在 `core/CMakeLists.txt` 里 `find_package` /
  `FetchContent` 后 `target_link_libraries(nbuv_core PRIVATE ...)`，
  绑定层会透明继承。
- **加 Python 依赖**：`uv add <pkg>`；仅开发时用的依赖 `uv add --group dev <pkg>`。

## 参考

- [nanobind 官方文档](https://nanobind.readthedocs.io/en/latest/)
- [scikit-build-core 文档](https://scikit-build-core.readthedocs.io/)
- [uv 文档](https://docs.astral.sh/uv/)

## License

采用 [Apache License 2.0](LICENSE)，全文见仓库根目录 `LICENSE` 文件。
