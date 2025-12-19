#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "Engine/Engine/Public/Engine.h"
namespace py = pybind11;

void start_engine()
{
    Engine engine;
    engine.run();
}

std::string say_hello() {
    return "Hello from C++!";
}

class Greeter {
public:
    Greeter(const std::string& name) : name_(name) {}

    void set_names(const std::vector<std::string>& names) {
        name_ = names[1];
    }

    void set_name(const std::string& name) {
        name_ = name;
    }

    std::string greet() const {
        return "Hello, " + name_ + "!";
    }

private:
    std::string name_;
};

PYBIND11_MODULE(Test_Pybind, m) {
    m.doc() = "A simple pybind11 example module";
    m.def("say_hello", &say_hello, "A function that returns a hello from C++");
    m.def("start_engine", &start_engine, "A function that start engine");
    py::class_<Greeter>(m, "Greeter")
        .def(py::init<const std::string&>(), py::arg("name"), "Construct a Greeter with a name")
        .def("greet", &Greeter::greet, "Greet the person")
        .def("set_name", &Greeter::set_name, py::arg("name"), "Set the name")
        .def("set_names", &Greeter::set_names, py::arg("names"), "Set the name");
    py::class_<Engine>(m, "Engine")
        .def(py::init<>(), "Create Engine")
        .def("mainloop", &Engine::mainLoop, "mainloop");
}
