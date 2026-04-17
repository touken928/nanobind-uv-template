// Python bindings for the nbuv C++ core library.
//
// This translation unit is the *only* place where Python / nanobind and the
// pure C++ API of `nbuv::` meet. Keep it thin: no business logic, only glue.

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/string_view.h>
#include <nanobind/stl/vector.h>

#include "nbuv/greeter.hpp"
#include "nbuv/math.hpp"

namespace nb = nanobind;
using namespace nb::literals; // enables the "name"_a shorthand

NB_MODULE(_core, m) {
    m.doc() = "Native core of nbuv (nanobind bindings over the nbuv C++ library).";

    m.def("add", &nbuv::add, "a"_a, "b"_a,
          "Return the sum of two integers.");
    m.def("sum", &nbuv::sum, "values"_a,
          "Return the sum of a sequence of floats.");

    nb::class_<nbuv::Greeter>(m, "Greeter",
        "A minimal demo class with a mutable `name` property.")
        .def(nb::init<>())
        .def(nb::init<std::string>(), "name"_a)
        .def("greet", &nbuv::Greeter::greet,
             "Return a friendly greeting.")
        .def_prop_rw("name",
                     &nbuv::Greeter::name,
                     &nbuv::Greeter::set_name,
                     "The name used in the greeting.");

    m.attr("__version__") = "0.1.0";
}
