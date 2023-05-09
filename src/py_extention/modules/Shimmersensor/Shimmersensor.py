import time
import asyncio

from datetime import datetime

from serial import Serial
from pyshimmer import ShimmerBluetooth, DEFAULT_BAUDRATE, DataPacket, EChannelType

import PyModuleProt_pb2 as proto

import os
import sys

# Add the path to the module to the system path
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

HOST = '127.0.0.1'        # Local host
PORT = 50008              # Arbitrary port

global streaming
streaming = False
class Shimmersensor: 
    shim_dev = None
    accel_ln_x = 0
    accel_ln_y = 0
    accel_ln_z = 0

    def Handler(self, pkt: DataPacket) -> None:
        accel_ln_x = pkt[EChannelType.ACCEL_LN_X]
        accel_ln_y = pkt[EChannelType.ACCEL_LN_Y]
        accel_ln_z = pkt[EChannelType.ACCEL_LN_Z]

    def StarteSensor(self):
        print("Starting sensor...")
        serial = Serial('COM5', DEFAULT_BAUDRATE)
        shim_dev = ShimmerBluetooth(serial)

        shim_dev.initialize()

        dev_name = shim_dev.get_device_name()
        print(f'Sensor name is: {dev_name}')

        shim_dev.add_stream_callback(handler)

        shim_dev.start_streaming()


    def StoppeSensor(self):
        print("Stopping sensor...")
        shim_dev.stop_streaming()
        shim_dev.shutdown()

streaming = False
sensor = Shimmersensor()

async def handle_client(reader, writer):
    addr = writer.get_extra_info('peername')
    print(f"New client connected: {addr}")
    
    async def send_data():
        global streaming
        global sensor 
        while True:
            if streaming:
                #print("Sending data...")
                pb = proto.DataPacket()
                pb.state = proto.State.STATE_STREAMING
                now = datetime.now()
                pb.timestamp = now.strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                pb.accel_ln_x = sensor.accel_ln_x 
                pb.accel_ln_y = sensor.accel_ln_y 
                pb.accel_ln_z = sensor.accel_ln_z 
                #print (pb.some_sensor)
                try:
                    writer.write(pb.SerializeToString())
                    await writer.drain()
                except ConnectionResetError:
                    print(f"Client {addr} disconnected")
                    break
            await asyncio.sleep(0.2)

    async def receive_data():
        global streaming
        global sensor 
        while True:
            try:
                data = await reader.read(1024)
                if not data:
                    print(f"Client {addr} disconnected")
                    streaming = False
                    break
            except ConnectionResetError:
                print(f"Client {addr} disconnected")
                streaming = False
                break

            print(f"Received data from {addr}: {data}")

            pb = proto.SendCommand()
            pb.ParseFromString(data)

            print(f"Command is: {pb.command}")
            if pb.command == proto.Command.COMMAND_START_STREAM:
                print("Starting stream...")
                sensor.StarteSensor()
                streaming = True
            elif pb.command == proto.Command.COMMAND_STOP_STREAM:
                print("Stopping stream...")
                sensor.StoppeSensor()
                streaming = False

    receive_task = asyncio.create_task(receive_data())
    send_task = asyncio.create_task(send_data())
    await asyncio.gather(receive_task, send_task)

async def main():
    server = await asyncio.start_server(handle_client, HOST, PORT)
    async with server:
        print("Server running on port " + str(PORT))
        await server.serve_forever()

asyncio.run(main())