#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <Wire.h>

void configureSlave(const byte address);
void configureMaster(const byte address);
void clearInterrupt(const byte mcpAddress);
unsigned int expanderRead(int address, byte data);

