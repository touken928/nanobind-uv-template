"""nbuv: nanobind bindings for the nbuv C++ core library."""

from ._core import *  # noqa: F401, F403
from ._core import __version__  # re-export (dunder, not pulled in by `*`)
