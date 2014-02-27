#include "Adafruit_MCP23017.h"
#include <Wire.h>

void clearInterrupt(int mcpAddress); 
void activateInterrup(int mcpAddress);
void setAllPortToInput(Adafruit_MCP23017 mcp, uint8_t enablePullUp);
