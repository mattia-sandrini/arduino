#ifndef SerialStreamFile_h
#define SerialStreamFile_h

#include "Arduino.h"

class SerialStreamFile {
  public:
    SerialStreamFile(Stream &streamObj);
    bool open(const char *path);
    int16_t read();
    bool seekSet(uint32_t pos);
    uint32_t curPosition(); 
  private:
    Stream &stream;
};

#endif