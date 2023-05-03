import time
import scripts.Shimmersensorptot_pb2 as proto

from serial import Serial

from pyshimmer import ShimmerBluetooth, DEFAULT_BAUDRATE, DataPacket, EChannelType



class Shimmersensor: 
    shim_dev = None
    sock = None
    conn = None
    addr = None
    pb = None

    def Handler(pkt: DataPacket) -> None:
        cur_value = pkt[EChannelType.ACCEL_LN_X]
        print(cur_value)
        pb.accel_ln_x = cur_value
        self.conn.sendall(pb.SerializeToString())
        print('Test data sent.')

    def StarteSensor():
        serial = Serial('COM5', DEFAULT_BAUDRATE)
        shim_dev = ShimmerBluetooth(serial)

        shim_dev.initialize()

        dev_name = shim_dev.get_device_name()
        print(f'My name is: {dev_name}')

        shim_dev.add_stream_callback(handler)

        shim_dev.start_streaming()


    def StoppeSensor():
        shim_dev.stop_streaming()
        shim_dev.shutdown()

    def startServer(self):
        print("Starting server...")
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((HOST, PORT))
        self.sock.listen(1)
        self.pb = proto.DataPacket()
        print("Server started.")

    def serverWaitConnection(self):
        print ('Waiting for connection...')
        self.conn, self.addr = self.sock.accept()

    def stopServer(self):
        self.conn.close()
        print ('Server closed.')


class ShimmerData:
    def init(self, accel_x=0.0):
        self.data = Shimmersensor_pb2.ShimmerData()
        self.data.accel.x = accel_x

    def serialize(self):
        return self.data.SerializeToString()

    def deserialize(self, data):
        self.data.ParseFromString(data)

    def set_values(self, accel_x):
        self.data.accel.x = accel_x