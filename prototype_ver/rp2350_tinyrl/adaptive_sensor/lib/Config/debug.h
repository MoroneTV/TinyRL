#ifndef DEBUG_H
#define DEBUG_H

#include "SensorWeights.h"

// DEBUG PRINTS, Stolen from user J_M_L on the arduino forums, found here:
// https://forum.arduino.cc/t/managing-serial-print-as-a-debug-tool/1024824/2

#define DEBUG 1    // SET TO 0 OUT TO REMOVE TRACES
#if DEBUG
#define D_SerialBegin(...) Serial.begin(__VA_ARGS__);
#define D_print(...)    Serial.print(__VA_ARGS__)
#define D_write(...)    Serial.write(__VA_ARGS__)
#define D_println(...)  Serial.println(__VA_ARGS__)
#else
#define D_SerialBegin(...)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#endif

#endif