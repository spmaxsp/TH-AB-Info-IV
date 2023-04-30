import time 
import socket

HOST = '127.0.0.1'        # Local host
PORT = 50007              # Arbitrary port

class PyModule:
    sock = None
    conn = None
    addr = None

    def __init__(self):
        globalVariable = 0

    def main(self):
        print("Hello World!")
        time.sleep(2)
        print("Goodbye World!")

    def someFunction(self):
        print("This is a function in the module")

    def someLoopingFunction(self):
        for i in range(10):
            print(i)
            time.sleep(1)

    def testGlobalVariable1(self):
        self.globalVariable = 123
        print("set global variable: " + str(self.globalVariable))

    def testGlobalVariable2(self):
        print("read global variable: " + str(self.globalVariable))

    def startServer(self):
        print("Starting server...")
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((HOST, PORT))
        self.sock.listen(1)
        print("Server started.")

    def serverWaitConnection(self):
        print ('Waiting for connection...')
        self.conn, self.addr = self.sock.accept()

    def serverSendData(self):
        self.conn.sendall(b'Hello, world')
        print('Test data sent.')

    def stopServer(self):
        self.conn.close()
        print ('Server closed.')
