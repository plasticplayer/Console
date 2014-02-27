#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "Communication.h"


#define GPINTENA 0x04   // Interrupt on change (0 = disable, 1 = enable)
#define GPINTENB 0x05


void expanderWriteBoth(int address, const byte reg, const byte data ) 
{
  Wire.beginTransmission (address);
  Wire.write (reg);
  Wire.write (data);  // port A
  Wire.write (data);  // port B
  Wire.endTransmission ();
}

void setAllPortToInput(Adafruit_MCP23017 mcp, uint8_t enablePullUp){
  for(int i=0; i<16;i++){
    mcp.pinMode(i, INPUT);         // Activer GPA1 comme entrée
    mcp.pullUp(1, enablePullUp);   // Activer la résistance Pull-Up interne de 100K 
  }
}

void activateInterrup(int mcpAddress){
  expanderWriteBoth(mcpAddress, GPINTENA, 0xFF);
}

void clearInterrupt(int mcpAddress){
  
}




