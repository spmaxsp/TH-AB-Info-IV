import time
from Shimmersensor_pb2 import datapacket 

from serial import Serial

from pyshimmer import ShimmerBluetooth, DEFAULT_BAUDRATE, DataPacket, EChannelType


def handler(pkt: DataPacket) -> None:
    cur_value = pkt[EChannelType.ACCEL_LN_X]
    print(cur_value)

def starteSensor():
    serial = Serial('COM5', DEFAULT_BAUDRATE)
    shim_dev = ShimmerBluetooth(serial)

    shim_dev.initialize()

    dev_name = shim_dev.get_device_name()
    print(f'My name is: {dev_name}')

    shim_dev.add_stream_callback(handler)

    shim_dev.start_streaming()
    
    
if __name__ == '__main__':
    
    starteSensor()
   
    shim_dev.stop_streaming()

    shim_dev.shutdown()
    input()

    


