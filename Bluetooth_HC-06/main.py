#!/usr/bin/env python
"""
A simple test server that returns a random number when sent the text "temp" via Bluetooth serial.
"""

import serial  # python -m pip install pyserial
import time

print("Hello")
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
