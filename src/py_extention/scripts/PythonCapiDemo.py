import time
import socket
import time

class PythonCapiDemo():
    def __init__(self):
        self.port = 0
        self.host = ''
        self.conn = False
        self.s = None

    def setup(self, portstr):
        print ('[INFO (Python)] Setup server...')
        self.port = int(portstr)
        self.host = 'localhost'
        self.conn = False

    def run(self):
        if self.conn is False:
            print ('[INFO (Python)] Run server...')
            self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.s.bind((self.host, self.port))
            self.s.listen(1)
            print ('[INFO (Python)] Waiting for connection...')
            self.conn, addr = self.s.accept()
            print ('[INFO (Python)] Connected by client'), addr

            print ('[INFO (Python)] Server is running...')

        else:
            print ('[INFO (Python)] Server is running in mainloop (script1)...')

def run():
    for i in range(15):
        print('printed from MyThread1...')
        time.sleep(1)

