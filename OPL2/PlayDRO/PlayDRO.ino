/**
 * This is an example sketch from the OPL2 library for Arduino. It plays a DRO audio file from SD card using the YM3812
 * audio chip.
 *
 * OPL2 board is connected as follows:
 *   Pin  8 - Reset
 *   Pin  9 - A0
 *   Pin 10 - Latch
 *   Pin 11 - Data
 *   Pin 13 - Shift
 *
 * Connect the SD card with Arduino SPI pins as usual and use pin 7 as CS.
 *
 * Refer to the wiki at https://github.com/DhrBaksteen/ArduinoOPL2/wiki/Connecting to learn how to connect your platform
 * of choice!
 *
 * By default this example will look for the phemopop.dro file in the root of the SD card. This file is found among the
 * files for this example. For more information about the DRO file format please visit
 * http://www.shikadi.net/moddingwiki/DRO_Format
 *
 * Code by Maarten Janssen (maarten@cheerful.nl) 2016-12-17
 * Song Phemo-pop! by Olli Niemitalo/Yehar 1996
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */

#include <SPI.h>
#include <OPL2.h>
//#include "bluetooth.h"
#include "SerialStreamFile.h"

void InitBluetoothDevice()
{
  char deviceName[] = "HardBlue";
  char devicePIN[]  = "1135";
  /*
  setBaudRate_115200(&Serial1); // Set the Baud rate between the PC and BT module to 115200, but between arduino and the module is still 9600
  delay(1000);
  //setBaudRate_9600N81(&Serial1);
  setDeviceName(&Serial1, deviceName);
  delay(1000);
  setDevicePIN(&Serial1, devicePIN);
  delay(1000);
  
  Serial1.print("AT+UART=9600,0,0");
  Serial1.write('\r');
  Serial1.write('\n');
  delay(1000);
  Serial1.print("AT+NAMEProva9");
  Serial1.write('\0');
  Serial1.write('\r');
  Serial1.write('\n');
  delay(1500);
  setDevicePIN(devicePIN);
  //Serial1.print("AT+PSWD1135");
  //Serial1.write('\r');
  //Serial1.write('\n');
  delay(1000);*/
}

OPL2 opl2;
SerialStreamFile droFile(Serial); // Serial1 (Bluetooth)

unsigned long offset       = 0;
unsigned long songLength   = 0;
unsigned long songDuration = 0;
byte codeShortDelay    = 0;
byte codeLongDelay     = 0;
byte registerMapLength = 0;
byte registerMap[256];


void setup() {
  Serial.begin(115200);
  Serial.println("Arduino is ON!");
  
  Serial1.begin(9600);   // This is always 9600 even when the baud rate is changed (that affect only the communication between the PC/Smartphone with the HC-06 module)
  //InitBluetoothDevice();

  opl2.init();
  
  //loadDroSong("phemopop.dro");
  loadDroSong("strikeforce.dro");
}


void loop() {
  unsigned long time = millis();

  while (songLength > 0) {
    int wait = playDroSong();

    if (wait > 0) {
      // Take into account time that was spent on IO.
      unsigned long ioTime = millis() - time;
      if (ioTime < wait) {
        delay(wait - ioTime);
      }
      time = millis();
    }

    songLength --;
  }
  droFile.close();
}


void loadDroSong(char* fileName) {
  droFile.open(fileName, FILE_READ);
  droFile.seek(12);

  songLength  = droFile.read();
  songLength += droFile.read() << 8;
  songLength += droFile.read() << 16;
  songLength += droFile.read() << 24;

  songDuration  = droFile.read();
  songDuration += droFile.read() << 8;
  songDuration += droFile.read() << 16;
  songDuration += droFile.read() << 24;

  droFile.seek(23);
  codeShortDelay = droFile.read();
  codeLongDelay  = droFile.read();
  registerMapLength = droFile.read();

  for (byte i = 0; i < registerMapLength; i ++) {
    registerMap[i] = droFile.read();
  }
}


int playDroSong() {
  byte code = droFile.read();
  byte data = droFile.read();

  if (code == codeShortDelay) {
    return data + 1;
  } else if (code == codeLongDelay) {
    return (data + 1) << 8;
  } else if (code < 128) {
    opl2.write(registerMap[code], data);
  }

  return 0;
}
