#!/usr/bin/env python
"""
A simple test server that returns a random number when sent the text "temp" via Bluetooth serial.
"""

import serial  # python -m pip install pyserial
import time
import asyncio  # requires Python 3.7+



rad_file = None
root_path = "./audiofiles/"



async def read_serial(connection):
    global rad_file
    input_data = connection.readline()
    cmd = input_data.decode().strip()
    cmd_code = cmd[0:2]
    if cmd_code == "OP":
        path = cmd[3:]
        #print("Opening \"{}\"".format(path))
        rad_file = open(root_path+path, "rb")
    elif cmd_code == "SK":
        pos = int(cmd[3:], 16)
        #print("Setting Seek at \"{}\"".format(pos)) # Convert from hex
        rad_file.seek(pos)
    elif cmd_code == "RD":
        data = rad_file.read(1)   # Read 1 byte
        #print("Read one byte: {}".format(data))
        connection.write(data)  # There's no need to encode it (it's already a byte)
    elif cmd_code == "CP":
        cur_pos = rad_file.tell()	# Returns the file's current position
        #print("File's Current Position: {}".format(cur_pos))
        connection.write(str.encode(str(cur_pos)))
    else:
        print("Message from Arduino: {}".format(cmd))


port = "COM3" # "COM6"
arduino = serial.Serial(port, 9600)
print("Connected to: {}".format(port))
arduino.flushInput()
arduino.flushOutput()
while True:
    # arduino.write(str.encode(str(i)))
    # time.sleep(0.2)
    # asyncio.run(read_serial(arduino))
    # continue

    #arduino.write(str.encode(str(''))) # Occorre quando si usa il bluetooth
    asyncio.run(read_serial(arduino))
    #time.sleep(0.1)
arduino.close()










exit(0)

port = "COM7"
bluetooth = serial.Serial(port, 9600)
print("Connected to: {}".format(port))
bluetooth.flushInput()
bluetooth.flushOutput()
for i in range(5):
    #bluetooth.write(b"BOOP "+str.encode(str(i)))
    bluetooth.write(str.encode(chr(ord('a') + i)))
    input_data = bluetooth.readline()
    print(input_data.decode())
    time.sleep(0.1)
bluetooth.close()
