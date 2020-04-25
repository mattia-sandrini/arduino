/**
 *
 * OPL2 board is connected as follows:
 *   Pin  8 - Reset
 *   Pin  9 - A0
 *   Pin 10 - Latch
 *   Pin 11 - Data
 *   Pin 13 - Shift

 * Refer to the wiki at https://github.com/DhrBaksteen/ArduinoOPL2/wiki/Connecting to learn how to connect your platform
 * of choice!
 *
 * For more information about the IMF file format please visit
 * http://www.shikadi.net/moddingwiki/IMF_Format
 *
 * Code is partially by Maarten Janssen (maarten@cheerful.nl) 2016-12-17
 * Songs from the games Bio Menace and Duke Nukem II by Bobby Prince
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

SerialStreamFile imfFile(Serial); // Serial1 (Bluetooth)

float imfSpeed;
long songLength = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Arduino is ON!");
  
  Serial1.begin(9600);   // This is always 9600 even when the baud rate is changed (that affect only the communication between the PC/Smartphone with the HC-06 module)
  //InitBluetoothDevice();

  opl2.init();

  imfSpeed = 560.0;
  loadImfSong("city.imf");

  //imfSpeed = 280.0;
  //loadImfSong("kickbuta.imf");
}

void loop() {  
  unsigned long time = millis();

  while (songLength > 0) {
    int wait = playImfSong();

    if (wait > 0) {
      // Take into account time that was spent on IO.
      unsigned long ioTime = millis() - time;
      if (ioTime < wait) {
        delay(wait - ioTime);
      }
      time = millis();
    }

    songLength--;
  }
  imfFile.close();
}


void loadImfSong(const char* fileName) {
  imfFile.open(fileName, FILE_READ);
  imfFile.seek(0);

  songLength  = imfFile.read();
  songLength += imfFile.read() << 8;
  if (songLength == 0) {
    songLength = 65535;
    imfFile.seek(4);
  }
}


int playImfSong() {
  byte reg  = imfFile.read();
  byte data = imfFile.read();
  float  wait = imfFile.read();
  wait += imfFile.read() << 8;

  if (reg != 0x00) {
    opl2.write(reg, data);
  }
  songLength -= 3;

  return round(wait * (1000 / imfSpeed));
}
