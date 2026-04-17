from __future__ import annotations

import nbuv as m


def test_version() -> None:
    assert isinstance(m.__version__, str)
    assert m.__version__


def test_add() -> None:
    assert m.add(1, 2) == 3
    assert m.add(-5, 5) == 0


def test_sum() -> None:
    assert m.sum([1.0, 2.0, 3.5]) == 6.5
    assert m.sum([]) == 0.0


def test_greeter_default() -> None:
    g = m.Greeter()
    assert g.name == "world"
    assert g.greet() == "Hello, world!"


def test_greeter_custom() -> None:
    g = m.Greeter("nanobind")
    assert g.greet() == "Hello, nanobind!"
    g.name = "uv"
    assert g.greet() == "Hello, uv!"
