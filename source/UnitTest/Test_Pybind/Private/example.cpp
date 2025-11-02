#include "pybind11/pybind11.h"

namespace py = pybind11;

std::string say_hello() {
    return "Hello from C++!";
}

class Greeter {
public:
    Greeter(const std::string& name) : name_(name) {}

    void set_name(const std::string& name) {
        name_ = name;
    }

    std::string greet() const {
        return "Hello, " + name_ + "!";
    }

private:
    std::string name_;
};

PYBIND11_MODULE(pyTest_Pybind, m) {
    m.doc() = "A simple pybind11 example module";
    m.def("say_hello", &say_hello, "A function that returns a hello from C++");
    py::class_<Greeter>(m, "Greeter")
        .def(py::init<const std::string&>(), py::arg("name"), "Construct a Greeter with a name")
        .def("greet", &Greeter::greet, "Greet the person")
        .def("set_name", &Greeter::set_name, py::arg("name"), "Set the name");
}
