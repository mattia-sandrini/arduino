#ifndef SerialStreamFile_h
#define SerialStreamFile_h

#include "Arduino.h"

struct Cache {
  uint32_t offset;  // Offset of te data from the beginning of the file
  uint16_t length;  // Lenght of data array
  byte data[255];    // Array of cached data
};

class SerialStreamFile {
  public:
    SerialStreamFile(Stream &streamObj);
    bool open(const char *path);
    int16_t read();
    bool seekSet(uint32_t pos);
    uint32_t curPosition();
    bool updateCache(Cache *cache, uint32_t offset, int length);
  private:
    Stream &stream;
    long curPos;
    Cache cachedDataHead;
    Cache cachedData;
};

#endif
