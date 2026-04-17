#pragma once

#include <string>
#include <string_view>

namespace nbuv {

// A minimal demo class used to showcase class bindings. Completely independent
// of Python / nanobind; the binding layer only translates calls to/from it.
class Greeter {
public:
    Greeter();
    explicit Greeter(std::string name);

    // Return a friendly greeting for the stored name.
    std::string greet() const;

    const std::string& name() const noexcept;
    void set_name(std::string_view name);

private:
    std::string name_;
};

} // namespace nbuv
