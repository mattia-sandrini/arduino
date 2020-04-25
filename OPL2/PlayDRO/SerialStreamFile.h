#ifndef SerialStreamFile_h
#define SerialStreamFile_h

#include "Arduino.h"

// According to the SD.h library
#define FILE_READ 1         
#define FILE_WRITE 23

struct Cache {
  uint32_t offset;  // Offset of te data from the beginning of the file
  uint16_t length;  // Lenght of data array
  byte data[255];    // Array of cached data
};

class SerialStreamFile {
  public:
    SerialStreamFile(Stream &streamObj);
    bool open(const char *path);
    bool open(const char *path, int mode);
    bool close();
    int16_t read();
    bool seek(uint32_t pos);
    bool seekSet(uint32_t pos);
    uint32_t curPosition();
    uint32_t position();
    bool updateCache(Cache *cache, uint32_t offset, int length);
  private:
    Stream &stream;
    long curPos;
    Cache cachedDataHead;
    Cache cachedData;
};

#endif
