import time 

class PyModule:
    def main(self):
        print("Hello World!")
        time.sleep(5)
        print("Goodbye World!")

    def someFunction(self):
        print("This is a function in the module")

    def someLoopingFunction(self):
        for i in range(10):
            print(i)
            time.sleep(1)