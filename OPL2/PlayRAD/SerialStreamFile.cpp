#include "SerialStreamFile.h"

#define CACHE_DATA_SIZE 40  // With the USB Serial 40 is a good value

/*  SerialStreamFile::SerialStreamFile(Stream &streamObj) {
  this->stream = streamObj;
}*/

SerialStreamFile::SerialStreamFile(Stream &streamObj):stream(streamObj) {}



bool SerialStreamFile::open(const char *path) {
  char str[256] = "OP:";
  strcat(str, path);
  this->stream.println(str);
  this->curPos = 0;

  this->updateCache(&this->cachedDataHead, this->curPos, 255);
  
  return true;
}

bool SerialStreamFile::open(const char *path, int mode) {
  if (mode == FILE_READ) {
    return this->open(path);
  }
  else {
    // Implement write mode
  }
}


bool SerialStreamFile::close() {
  this->stream.println("FN");
  return true;
}
    
int16_t SerialStreamFile::read() {
  int16_t relativePosHead = this->curPos - this->cachedDataHead.offset;
  int16_t relativePos = this->curPos - this->cachedData.offset;
  if (relativePos < this->cachedData.length) {   // If the cached data covers the portion of file to be read now
    this->curPos++;             // At this point there is a mismatch with the position of the source
    return this->cachedData.data[relativePos]; // Access the cached data
  }
  else if (relativePosHead < this->cachedDataHead.length) {
    this->curPos++;
    return this->cachedDataHead.data[relativePosHead];
  }
  else {
    this->updateCache(&this->cachedData, this->curPos, CACHE_DATA_SIZE);  // Update te cache and call the read method which should now access the cached data
    return this->read(); 
    
    /*this->stream.println("RD");
    while (!this->stream.available())
      delayMicroseconds(1); // __ams__("nop\n\t"); // Wait roughly 62ns
    int16_t data = this->stream.read();
    this->curPos++;
    return data;*/
  }
}

bool SerialStreamFile::seek(uint32_t pos) {
  return this->seekSet(pos);
}

bool SerialStreamFile::seekSet(uint32_t pos) {
  int16_t relativePosHead = pos - this->cachedDataHead.offset;
  int16_t relativePos = pos - this->cachedData.offset;
  if (relativePosHead >= 0 && relativePosHead < this->cachedDataHead.length) { // It wants to access the head of the file
    this->curPos = pos;
    return true;
  }
  else if (relativePos >= 0 && relativePos < this->cachedData.length) {   // If the cached data covers the portion of file where to move to
    this->curPos = pos;             // At this point there is a mismatch with the position of the source
    return true;
  }
  else {
    char str[14] = "SK:";
    sprintf(str+3, "%X", pos); // Skip the first 3 character of the string and start writing after "SK:"
    this->stream.println(str);
    this->curPos = pos;
  
    this->updateCache(&this->cachedData, this->curPos, CACHE_DATA_SIZE);
    
    return true;
  }
  /*char str[14] = "SK:";
  sprintf(str+3, "%X", pos); // Skip the first 3 character of the string and start writing after "SK:"
  this->stream.println(str);
  this->curPos = pos;
  return true;*/
}

uint32_t SerialStreamFile::curPosition() {
  return this->curPos;
  /*this->stream.println("CP");
  while (!this->stream.available())
    delayMicroseconds(1); // __ams__("nop\n\t"); // Wait roughly 62ns
  return this->stream.parseInt();*/
}

uint32_t SerialStreamFile::position() {
  return this->curPosition();
}

bool SerialStreamFile::updateCache(Cache *cache, uint32_t offset, int len) {
  //this->stream.print("RC:");
  char str[14] = "RC:";
  sprintf(str+3, "%X", len); // Skip the first 3 character of the string and start writing after "SK:"
  this->stream.println(str);

  cache->offset = offset;
  cache->length = len;
  
  while (!this->stream.available())
    delayMicroseconds(1); // __ams__("nop\n\t"); // Wait roughly 62ns
  //delay(3);
  // Serial.readBytesUntil(character, buffer, length) -> character: the character to search for; buffer: the buffer to store the bytes in;
  
  this->stream.readBytes(cache->data, len); 
  //byte temp[len];
  //this->stream.readBytes(temp, len); 

  //this->stream.write(this->cachedData.data, len);
  //this->stream.write('\n');
  /*this->stream.write(temp, len);
  this->stream.write('\n\);
  this->stream.println("Ho cachato");*/
  return true;
}
