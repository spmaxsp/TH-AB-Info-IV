import time
import asyncio

from datetime import datetime

from serial import Serial
import live_advance 

import EEG_pb2 as proto

import os
import sys


global streaming
streaming = False


class EEG: 

    def StarteSensor(self):
        your_app_client_id = 'lcP2cs4kNNbl2ps9F8TeVDRiw59S1xq1APhphqzg'
        your_app_client_secret = 'LKcNIrVNbvtob0qGVg5dSm45nxoLaAiDj8qfW1HjiPGTe4QgowIGzFuOXXMCy5d7xuXcateRNNy38GPZO8KtxKdfJiPfxd8Vh6OkAV5iZ6qF9PpK1KsepVU1YqRFDMg3'

    
        livesensor = LiveAdvance(your_app_client_id, your_app_client_secret)     

        trained_profile_name = 'ayberk'
        livesensor.start(trained_profile_name)

if __name__ =='__main__':
    def StoppeSensor(self):
        livesensor.c.unsub_request(['com'])
        livesensor.c.close()

        print("Reading Mental Command data stopped.")

streaming = False
sensor = EEG()

async def handle_client(reader, writer):
    global sensor
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
                #print (sensor.accel_ln_x)
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
                print("running")
                streaming = True
                print("Sensor running")
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
