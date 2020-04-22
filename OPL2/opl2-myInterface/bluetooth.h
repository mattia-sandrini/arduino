#ifndef bluetooth_h
#define bluetooth_h

#include "Arduino.h"

void sendCommand(Stream *S, char* c);

void setBaudRate_9600N81(Stream *S);

void setDeviceName(Stream *S, char* name);

void setDevicePIN(Stream *S, char* pin);

#endif