#include "nbuv/greeter.hpp"

namespace nbuv {

Greeter::Greeter() : name_("world") {}

Greeter::Greeter(std::string name) : name_(std::move(name)) {}

std::string Greeter::greet() const {
    return "Hello, " + name_ + "!";
}

const std::string& Greeter::name() const noexcept {
    return name_;
}

void Greeter::set_name(std::string_view name) {
    name_.assign(name);
}

} // namespace nbuv
