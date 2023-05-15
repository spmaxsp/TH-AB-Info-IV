import asyncio
import random

from datetime import datetime

import PyModuleProt_pb2 as proto

import os
import sys

# Add the path to the module to the system path
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

HOST = '127.0.0.1'        # Local host
PORT = 50007              # Arbitrary port

global streaming
streaming = False

async def handle_client(reader, writer):
    addr = writer.get_extra_info('peername')
    print(f"New client connected: {addr}")
    
    async def send_data():
        global streaming
        while True:
            if streaming:
                print("Sending data...")
                pb = proto.DataPacket()
                pb.state = proto.State.STATE_STREAMING
                now = datetime.now()
                pb.timestamp = now.strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                pb.some_sensor = random.randint(0, 100)
                #print (pb.some_sensor)
                try:
                    writer.write(pb.SerializeToString())
                    await writer.drain()
                except ConnectionResetError:
                    print(f"Client {addr} disconnected")
                    break
            await asyncio.sleep(1)

    async def receive_data():
        global streaming
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

            print(f"Received data from {addr}: {data.decode()}")

            pb = proto.SendCommand()
            pb.ParseFromString(data)

            if pb.command == proto.Command.COMMAND_START_STREAM:
                print("Starting stream...")
                streaming = True
            elif pb.command == proto.Command.COMMAND_STOP_STREAM:
                print("Stopping stream...")
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

