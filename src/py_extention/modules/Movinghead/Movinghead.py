import asyncio
from pyartnet import ArtNetNode
from datetime import datetime

import MovingheadProt_pb2 as proto

import os
import sys

# Add the path to the module to the system path
sys.path.append(os.path.dirname(os.path.realpath(__file__)))

HOST = '127.0.0.1'  # Local host
PORT = 50009  # Arbitrary port

global rechts
global links
global oben
global unten
global normal
global parameter

rechts = False
links = False
oben = False
unten = False    
normal = False
parameter = 1

async def movingheadctr():
    global rechts
    global links
    global oben
    global unten
    global normal
    global parameter

    # Run this code in your async function
    node = ArtNetNode('127.0.0.1', 6454)
    print("Start")

    # Create universe 0
    universe = node.add_universe(0)

    #Deklaration der einzelnen Kanäle:
    channel_pan = universe.add_channel(start=1, width=1)        #Pan (grob) Kanal 1
    channel_panfine = universe.add_channel(start=2, width=1)    #Pan (fein) Kanal 2
    channel_tilt = universe.add_channel(start=3, width=1)       #Tilt (grob) kanal 3
    channel_tiltfine = universe.add_channel(start=4, width=1)   #Tilt (fein) Kanal  4
    channel_velocity = universe.add_channel(start=5, width=1)   #Geschwindigkeit Kanal 5
    channel_dimmer = universe.add_channel(start=6, width=1)     #Dimmer Kanal 6
    channel_strobe = universe.add_channel(start=7, width=1)     #Strobe-Effekt Kanal 7
    channel_color = universe.add_channel(start=8, width=1)      #Farbe Kanal 8
    channel_gobo = universe.add_channel(start=9, width=1)       #Gobo Kanal 9
    channel_special = universe.add_channel(start=10, width=1)   #Besondere Einstellungen Kanal 10


    #Einstellung der Parameter für die Kanäle (am Start):
    panwert = 85
    tiltwert = 15

    channel_gobo.set_values([0])        #Gobo offen
    channel_strobe.set_values([0])      #Strobo Aus
    channel_panfine.set_values([0])
    channel_tiltfine.set_values([0])
    channel_special.set_values([0])     #Besondere Einstellungen Aus
    channel_dimmer.set_values([255])    #Dimmer wird auf 100% gestellt also 255
    channel_velocity.set_values([200])  #Geschwindigkeit 200 (255=langsam)
    channel_pan.set_values([5])         #Anfangspositon horizontal
    channel_tilt.set_values([15])       #Anfangsposition vertikal
    channel_color.set_values([50])      #Farbe Rot
    await asyncio.sleep(2)              #5

    while True:
        #Ausrichtung des Moving-Head entsprchend des Eingangswert:
        if rechts:
            print("rechts")
            if panwert < parameter:
                panwert = 170 - parameter + panwert
                channel_pan.set_values([panwert])
            else:
                channel_pan.set_values([panwert - parameter])  #Weiterdrehen
                panwert = panwert - parameter
            await asyncio.sleep(0.1)
            rechts = False

        if oben:
            print("oben")
            if tiltwert + parameter > 127:#da Senkrechte ansonsten überschritten wird (sonst dreht sich oben unten)
                if panwert > 170: #Verhinderung Wertebereichsüberschreitung
                    panwert = panwert - 85 #Drehung um 180° zurück
                    channel_pan.set_values([panwert])
                    await asyncio.sleep(0.1)
                    tiltwert = 127  #127 - parameter + 127 - tiltwert
                    channel_tilt.set_values([tiltwert])
                    await asyncio.sleep(0.1)
                    oben = False
                else:
                    panwert = panwert + 85  #Drehung um 180° vor
                    channel_pan.set_values([panwert])
                    await asyncio.sleep(0.1)
                    tiltwert = 127  #127 - parameter + 127 - tiltwert
                    channel_tilt.set_values([tiltwert])
                    await asyncio.sleep(0.1)
                    oben = False
            else:#Weiterkippen verhindern wenn Senkrechte überschritten
                channel_tilt.set_values([tiltwert + parameter])
                await asyncio.sleep(0.1)
                tiltwert = tiltwert + parameter
                oben = False



        if unten:
            print("unten")
            if tiltwert >= parameter: #Verhindern das Tilt unter 0 gelangt
                channel_tilt.set_values([tiltwert - parameter])
                await asyncio.sleep(0.1)
                tiltwert = tiltwert - parameter
                unten = False
            else:
                tiltwert = 0
                channel_tilt.set_values([tiltwert])
                await asyncio.sleep(0.1)
                unten = False



        if links:
            print("links")
            if panwert + parameter > 255:
                panwert = panwert - 170 + parameter
                channel_pan.set_values([panwert])
                await asyncio.sleep(0.1)
                links = False
            else:
                channel_pan.set_values([panwert + parameter])
                await asyncio.sleep(0.1)
                panwert = panwert + parameter
                links = False



        if normal:
            print("normal")
            panwert = 85
            tiltwert = 15
            channel_pan.set_values([panwert])
            channel_tilt.set_values([tiltwert])
            await asyncio.sleep(0.1)
            normal = False

        await asyncio.sleep(0.5)

async def handle_client(reader, writer):
    addr = writer.get_extra_info('peername')
    print(f"New client connected: {addr}")

    async def receive_data():
        print("Recieve loop started")

        global rechts
        global links
        global oben
        global unten
        global normal
        global parameter

        while True:
            try:
                data = await reader.read(1024)
                if not data:
                    print(f"Client {addr} disconnected")
                    break
            except ConnectionResetError:
                print(f"Client {addr} disconnected")
                break

            print(f"Received data from {addr}: {data.decode()}")

            pb = proto.SendCommand()
            pb.ParseFromString(data)

            if pb.command == proto.Command.COMMAND_MOVE_RIGHT:
                print("Move right...")
                rechts = True
            elif pb.command == proto.Command.COMMAND_MOVE_LEFT:
                print("Move left...")
                links = True

            elif pb.command == proto.Command.COMMAND_MOVE_UP:
                print("Move up...")
                oben = True

            elif pb.command == proto.Command.COMMAND_MOVE_DOWN:
                print("Move down...")
                unten = True

            elif pb.command == proto.Command.COMMAND_MOVE_NORMAL:
                print("Move normal...")
                normal = True

            elif pb.command == proto.Command.COMMAND_SET_STEP:
                print("Set Step...")
                try:
                    parameter = pb.value
                except:
                    print("Invalid...")
            #asyncio.sleep(0.4)

    receive_task = asyncio.create_task(receive_data())
    dmx_task = asyncio.create_task(movingheadctr())
    await asyncio.gather(receive_task, dmx_task)


async def main():
    server = await asyncio.start_server(handle_client, HOST, PORT)
    async with server:
        print("Server running on port " + str(PORT))
        await server.serve_forever()

asyncio.run(main())

