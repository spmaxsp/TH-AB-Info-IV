import time
import asyncio

from datetime import datetime

import cortex 
from cortex import Cortex

import EEGProt_pb2 as proto

import os
import sys

#your_app_client_id = 'lcP2cs4kNNbl2ps9F8TeVDRiw59S1xq1APhphqzg'
#your_app_client_secret = 'LKcNIrVNbvtob0qGVg5dSm45nxoLaAiDj8qfW1HjiPGTe4QgowIGzFuOXXMCy5d7xuXcateRNNy38GPZO8KtxKdfJiPfxd8Vh6OkAV5iZ6qF9PpK1KsepVU1YqRFDMg3'

#HOST = '172.0.0.1'
#PORT = 65432

your_app_client_id = sys.argv(3)
your_app_client_secret = sys.argv(4)

HOST = sys.argv(1)
PORT = sys.argv(0)

class EEG: 
    mental_command = ""
    ready_state = False 
    profile_list = []
    active_profile = ""

    def __init__(self, app_client_id, app_client_secret, **kwargs):
        self.c = Cortex(app_client_id, app_client_secret, debug_mode=True, **kwargs)
        self.c.bind(create_session_done=self.on_create_session_done)
        self.c.bind(query_profile_done=self.on_query_profile_done)
        self.c.bind(load_unload_profile_done=self.on_load_unload_profile_done)
        #self.c.bind(save_profile_done=self.on_save_profile_done)
        self.c.bind(new_com_data=self.on_new_com_data)
        #self.c.bind(get_mc_active_action_done=self.on_get_mc_active_action_done)
        #self.c.bind(mc_action_sensitivity_done=self.on_mc_action_sensitivity_done)
        self.c.bind(inform_error=self.on_inform_error)


    def StarteSensor(self):
        trained_profile_name = 'ayberk'
        self.c.set_wanted_profile(self.active_profile)
        self.c.open()
        print("Reading Mental Command data started.")


    def StoppeSensor(self):
        self.c.close()
        print("Reading Mental Command data stopped.")

    def LoadProfile(self, profile_name):
        self.c.setup_profile(profile_name, 'load')

    def UnloadProfile(self, profile_name):
        self.c.setup_profile(profile_name, 'unload')

    #callbacks 
    def on_create_session_done(self, *args, **kwargs):
        print('session created')
        self.c.query_profile() 

    
    def on_query_profile_done(self, *args, **kwargs):
        print('on_query_profile_done')
        self.profile_list = [kwargs.get('data')]
        self.ready_state = True 


    def on_new_com_data(self, *args, **kwargs):
        data = kwargs.get('data')
        print('mc data: {}'.format(data))


    def on_inform_error(self, *args, **kwargs):
        error_data = kwargs.get('error_data')
        error_code = error_data['code']
        error_message = error_data['message']

        print(error_message)

    def on_load_unload_profile_done(self, *args, **kwargs):
        is_loaded = kwargs.get('isLoaded')
        print("on_load_unload_profile_done: " + str(is_loaded))
        
        if is_loaded == True:
            # get active action
            self.get_active_action(self.profile_name)
        else:
            print('The profile ' + self.profile_name + ' is unloaded')
            self.profile_name = ''


async def handle_client(reader, writer):
    global sensor
    addr = writer.get_extra_info('peername')
    print(f"New client connected: {addr}")

    
    def send_data_once():
        global sensor
        pb = proto.DataPacket()
        pb.type = proto.Type.TYPE_STATE_ONLY
        if sensor.ready_state: 
            pb.state = proto.State.STATE_READY
        else:
            pb.state = proto.State.STATE_WAITING
            pb.data = ""
            pb.data = ','.join(sensor.profile_list)
        now = datetime.now()
        pb.timestamp = now.strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
        try:
            writer.write(pb.SerializeToString())
        except ConnectionResetError:
            print(f"Client {addr} disconnected") 

    
    async def send_data_loop():
        global streaming
        global sensor 
        while True:
            if streaming:
                #print("Sending data...")
                pb = proto.DataPacket()
                pb.state = proto.State.STATE_STREAMING
                pb.data = sensor.mental_command
                now = datetime.now()
                pb.timestamp = now.strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
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

            elif pb.command == proto.Command.COMMAND_LOAD_PROFILE:
                print("Loading profile")
                sensor.profile_name = pb.data 
                print(pb.data)
            elif pb.command == proto.Command.COMMAND_GET_STATE:
                print("Sending state")
                send_data_once()

    receive_task = asyncio.create_task(receive_data())
    send_task = asyncio.create_task(send_data_loop())
    await asyncio.gather(receive_task, send_task)


async def main():
    server = await asyncio.start_server(handle_client, HOST, PORT)
    async with server:
        print("Server running on port " + str(PORT))
        await server.serve_forever()

streaming = False
sensor = EEG(your_app_client_id, your_app_client_secret)

asyncio.run(main())
