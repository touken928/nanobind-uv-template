<h1 align="center">nanobind-uv-template</h1>

<p align="center">
  <strong>GitHub 模板：用 <a href="https://nanobind.readthedocs.io/">nanobind</a> + <a href="https://cmake.org/">CMake</a> + <a href="https://docs.astral.sh/uv/">uv</a> 做「C++ 核心 + Python 包」。纯 C++ 在 <code>libs/</code>，示例发行名 <code>nbuv</code> 在仓库根目录。</strong>
</p>

<p align="center">
  <a href="https://www.python.org/downloads/"><img src="https://img.shields.io/badge/python-3.12-blue.svg?style=for-the-badge&logo=python" alt="Python 3.12"></a>
  <a href="https://en.cppreference.com/w/cpp/17"><img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++17"></a>
  <a href="https://github.com/touken928/nanobind-uv-template/actions/workflows/ci.yml"><img src="https://img.shields.io/github/actions/workflow/status/touken928/nanobind-uv-template/ci.yml?branch=main&logo=github&label=CI&style=for-the-badge" alt="CI"></a>
</p>

<p align="center"><a href="README.md">English</a></p>

构建后端：[`scikit-build-core`](https://scikit-build-core.readthedocs.io/)。模板仓库：<https://github.com/touken928/nanobind-uv-template>。

示例包 **`nbuv`** 贯穿 PyPI 名、`import`、CMake target（`nbuv::nbuv`）；clone 后改成你的名字。

## 使用模板

1. GitHub 上 **Use this template** 或 `git clone … my-pkg`。
2. 改目录名并全局替换引用：

   ```bash
   git mv libs/nbuv libs/<your_pkg>
   git mv src/nbuv src/<your_pkg>
   rg -l '\bnbuv\b'
   ```

3. 改根目录 `pyproject.toml`（`[project]`、`wheel.packages`、链接、版本）及 CMake / C++ 命名。
4. `uv sync`。

## 目录

```
nanobind-uv-template/
├── pyproject.toml
├── CMakeLists.txt
├── bindings/           # nanobind → nbuv._core
├── src/nbuv/
├── tests/
└── libs/nbuv/          # nbuv::nbuv（纯 C++）
    ├── CMakeLists.txt
    ├── include/nbuv/
    └── src/
```

依赖方向：`libs/nbuv` ← `bindings/` ← `src/nbuv`。`wheel.packages` 路径最后一级须与包名一致（示例 `nbuv = src/nbuv`）。

## 环境

C++17、CMake 3.15+、[uv](https://docs.astral.sh/uv/getting-started/installation/)。

## Python

```bash
uv sync
uv run pytest tests
uv build          # 输出到 ./dist/
```

修改 `libs/nbuv/**` 或 `bindings/**` 后，下次 `uv sync` 会触发重编译（`[tool.uv]` cache-keys）。稳定 ABI wheel 面向 Python 3.12（`requires-python` 见 `pyproject.toml`）。

## 只编 C++

```bash
cmake -S libs/nbuv -B build-nbuv
cmake --build build-nbuv -j
```

```cmake
add_subdirectory(path/to/.../libs/nbuv)
target_link_libraries(my_app PRIVATE nbuv::nbuv)
```

## CI 与发版

- **`main` 上 PR / push：** [`.github/workflows/ci.yml`](.github/workflows/ci.yml)，Ubuntu 与 `macos-15-intel` 上跑 `pytest`。
- **打 `v*` tag：** [`.github/workflows/release.yml`](.github/workflows/release.yml)（GitHub Release）与 [`.github/workflows/pypi.yml`](.github/workflows/pypi.yml)（PyPI）。PyPI 侧配置 [Trusted Publishing](https://docs.pypi.org/trusted-publishers/)（workflow `pypi.yml`、environment `pypi`），或使用仓库密钥 `PYPI_API_TOKEN`（见 `pypi.yml`）。tag 名含 `-` 时跳过 PyPI（预发布）。

```bash
uv version X.Y.Z
git commit -am "Release vX.Y.Z"
git tag vX.Y.Z && git push && git push --tags
```

安装：`pip install nbuv`，或在 [Releases](https://github.com/touken928/nanobind-uv-template/releases/latest) 下载 wheel。

## 扩展

- C++：在 `libs/nbuv` 加源码并写入 `add_library(nbuv …)`；用 `tests/` 测。
- Python 接口：`bindings/_core.cpp` 与 `src/nbuv/`。
- 依赖：`target_link_libraries(nbuv …)` / `uv add`。

## 文档

[nanobind](https://nanobind.readthedocs.io/en/latest/) · [scikit-build-core](https://scikit-build-core.readthedocs.io/) · [uv](https://docs.astral.sh/uv/)

## 许可证

示例分发 **nbuv**：[Apache-2.0](LICENSE)。
