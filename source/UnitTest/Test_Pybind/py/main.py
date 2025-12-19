import Test_Pybind

class A (Test_Pybind.Engine):
    def hhh(self):
        print("hhh")

engine = A()
engine.hhh()
while True:
    engine.mainloop()