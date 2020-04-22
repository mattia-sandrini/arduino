#include "SerialStreamFile.h"


/*  SerialStreamFile::SerialStreamFile(Stream &streamObj) {
  this->stream = streamObj;
}*/

SerialStreamFile::SerialStreamFile(Stream &streamObj):stream(streamObj) {}

bool SerialStreamFile::open(const char *path) {
  char str[256] = "OP:";
  strcat(str, path);
  this->stream.println(str);
  this->curPos = 0;
  return true;
}
    
int16_t SerialStreamFile::read() {
  this->stream.println("RD");
  while (!this->stream.available())
    delayMicroseconds(1); // __ams__("nop\n\t"); // Wait roughly 62ns
  int16_t data = this->stream.read();
  this->curPos++;
  return data;
}
    
bool SerialStreamFile::seekSet(uint32_t pos) {
  char str[14] = "SK:";
  sprintf(str+3, "%X", pos); // Skip the first 3 character of the string and start writing after "SK:"
  this->stream.println(str);
  this->curPos = pos;
  return true;
}

uint32_t SerialStreamFile::curPosition() {
  return this->curPos;
  /*this->stream.println("CP");
  while (!this->stream.available())
    delayMicroseconds(1); // __ams__("nop\n\t"); // Wait roughly 62ns
  return this->stream.parseInt();*/
}
