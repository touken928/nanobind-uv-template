# nbuv

Example package for [`nanobind-uv-template`](https://github.com/touken928/nanobind-uv-template).

`nbuv` is a small Python package whose core logic lives in the pure-C++
library `libs/nbuv` (target `nbuv::nbuv`); this package provides the nanobind
glue that exposes that library as the `nbuv` Python module.

See the [repository README](https://github.com/touken928/nanobind-uv-template)
for the full layout and developer workflow.

## Install

```bash
pip install nbuv
```

## Usage

```python
import nbuv

nbuv.add(1, 2)                         # -> 3
nbuv.sum([1.0, 2.0, 3.5])              # -> 6.5
nbuv.Greeter("nanobind").greet()       # -> 'Hello, nanobind!'
```

## License

Apache-2.0. See `LICENSE`.
