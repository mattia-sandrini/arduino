#!/usr/bin python

import serial  # python -m pip install pyserial
import time
import asyncio  # requires Python 3.7+
import os

root_path = "./audiofiles/"
root_path = "./audiofiles/RAD/"
# root_path = "./audiofiles/IMF/"
root_path = "./audiofiles/VGM/"
root_path = "./audiofiles/VGM/UltimaVI/"
root_path = "./audiofiles/VGM/Vimana_(Toaplan_1)/"          # 11
root_path = "./audiofiles/VGM/The_Light_Corridor_(PC)/"     # 0
root_path = "./audiofiles/VGM/The_Secret_of_Monkey_Island_(IBM_PC_AT)/"     # 0,
# root_path = "./audiofiles/VGM/The_Real_Ghostbusters_(DECO8)/"     # 1, 0
#
# root_path = "./audiofiles/VGM/The_Amazing_Spider-Man_and_Captain_America_in_Dr._Doom's_Revenge_(IBM_PC_AT)/"
# root_path = "./audiofiles/VGM/KGB_(IBM_PC_AT)/"
# root_path = "./audiofiles/VGM/Adlib_Music_Synthesizer_Card_Demo_Songs_(IBM_PC_XT_AT)/"
# # 14, 9, 4, 5, 6, 85, 92
#
# root_path = "./audiofiles/VGM/Dragon_Slayer_-_The_Legend_of_Heroes_(IBM_PC_AT)/"
# # 0

root_path = "./audiofiles/VGM/The_Simpsons_(IBM_PC_AT)/"     # 0
root_path = "./audiofiles/VGM/Castlevania_(IBM_PC_XT_AT)/"
root_path = "./audiofiles/VGM/Alone_in_the_Dark_(PC)/"
root_path = "./audiofiles/VGM/Bubble_Bobble_(IBM_PC_XT_AT)/"

# root_path = "./audiofiles/DRO/"
filenames = []
entries = os.listdir(root_path)
i=0
for entry in entries:
    print("%d: %s" % (i, entry))
    filenames.append(entry)
    i += 1
#exit(0)

index_file = 1
rad_file = None

# Read 255 bytes: b'RAD by REALiTY!!!\x06"Waterfall"\x01-----------\x01\x01RAD2 tune by VOID/REALITY in April 2018\x01(original Amiga version by 4mat/Anarchy)\x01\x01# Feel free to use this in your productions, just DON\'T FORGET THE CREDITS!!! #\x00\x01\x11String Chords LowLf@@\x00\x05\x80\xf3\x06\x00\x11\x80\xf2\x00\x00\x00\x80\xf3\x00\x00\x11\x80\xf2\x00\x02\x12Strin'
# Read 255 bytes: b'RAD by REALiTY!!\x10\x86Starport BBS Introtune\x01\x01By Purple Motion/FC - 1992\x01This is pure Adlib so\x01don\'t get excited..\x01\x01\x01Converted from .S3M for the release of "RAD" by REALITY\x00\x01\x00\x00\x00\xc0\xd2\xf8\x1f\xf0\n\x00\x00\x02\xd0T\x02\x00\xf5\xf3\x7f\xff\x0e\x00\x00\x03\x00\x0f@\x00\xe7\xf7\xf0\xf0\x08\x03\x00\x06\x00\x00\x01\x01\xf2\xf2\xf0\xf0\x00\x01\x02\x07c\x01\x00L\xf3\xf2\xf0P\n\x01\x02\x08\x00\x00\x00\x08\xf4\xf8G&\x04\x00\x02\t\x01\x00\x00\x06\xe5\xff\xf0#\x00\x00\x01\n@'



async def read_serial(connection):
    global rad_file
    input_data = connection.readline()
    try:
        cmd = input_data.decode().strip()
        cmd_code = cmd[0:2]
    except UnicodeDecodeError as e:
        cmd = str(input_data)
        cmd_code = None

    if cmd_code == "OP":
        path = cmd[3:]
        path = filenames[index_file]
        print("Opening \"{}\"".format(path))
        rad_file = open(root_path+path, "rb")
    elif cmd_code == "SK":
        pos = int(cmd[3:], 16)
        print("Setting Seek at \"{}\"".format(pos)) # Convert from hex
        rad_file.seek(pos)
    elif cmd_code == "RD":
        data = rad_file.read(1)   # Read 1 byte
        connection.write(data)  # There's no need to encode it (it's already a byte)
        print("Read one byte: {}".format(data))
    elif cmd_code == "CP":
        cur_pos = rad_file.tell()	# Returns the file's current position
        connection.write(str.encode(str(cur_pos)))
        print("File's Current Position: {}".format(cur_pos))
    elif cmd_code == "RC": # Request Cache
        length = int(cmd[3:], 16)
        cur_pos = rad_file.tell()
        data = rad_file.read(length)   # Read N byte
        # Uncommenting the line below would create a mismatch of positions between arduino and python
        #rad_file.seek(cur_pos)         # Reset the file's cursor to the same position as before
        connection.write(data)
        print("Read {} bytes: {}".format(length, data))
    elif cmd_code == "FN":
        #connection.close()
        exit(0)
    else:
        print("Message from Arduino: {}".format(cmd))


port = "COM3" # "COM6"
arduino = serial.Serial(port, 115200) # 115200 both for USB and Bluetooth

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










# exit(0)
#
# port = "COM7"
# bluetooth = serial.Serial(port, 9600)
# print("Connected to: {}".format(port))
# bluetooth.flushInput()
# bluetooth.flushOutput()
# for i in range(5):
#     #bluetooth.write(b"BOOP "+str.encode(str(i)))
#     bluetooth.write(str.encode(chr(ord('a') + i)))
#     input_data = bluetooth.readline()
#     print(input_data.decode())
#     time.sleep(0.1)
# bluetooth.close()
