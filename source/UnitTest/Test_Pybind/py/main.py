import pyTest_Pybind

print(pyTest_Pybind.say_hello())

greeter = pyTest_Pybind.Greeter("World")
print(greeter.greet())

greeter.set_name("Alice")
print(greeter.greet())
