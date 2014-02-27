
// Test de base de lecture d'entrée (et activation de la Pull-Up) sur un MCP23017 I/O expander
// Domaine Publique! (code d'origine)

// Connectez la broche #12 du MCP23017 sur Arduino broche Analogique A5 (Horloge I2C, SCL)
// Connectez la broche #13 du MCP23017 sur Arduino broche Analogique A4 (Données I2C, SDA)
// Connectez la broche #15, 16 et 17 du MCP23017 sur DNG (Sélection de l'adresse)
// Connectez la broche #9 du MCP23017 sur 5V (Alimentation)
// Connectez la broche #10 du MCP23017 sur GND (Masse commune)

// Entrée #1 (GPA1, broche 22) est connectée sur bouton puis sur la masse 
// par l'intermédiaire d'une résistance de 330 Ohms

// Ajout MCHobby:
// - Connectez la broche #18 du MCP23017 sur 5V (désactiver la ligne Reset)


#include <Wire.h>
#include "Adafruit_MCP23017.h"
#include "def.h" // Include
#include "Communication.h"

#define PIN_INTERRUPT_A 22
#define PIN_INTERRUPT_B 24

#define PORT_A 0
#define PORT_B 255

#define SPEED_SERIE 9600
#define ENABLE_PULLUP HIGH

#define NUMBER_MCP 6
#define ADDRESS_MCP_MASTER 0b000
#define ADDRESS_MCP_SLAVE1 0b001
#define ADDRESS_MCP_SLAVE2 0b010
#define ADDRESS_MCP_SLAVE3 0b011
#define ADDRESS_MCP_SLAVE4 0b100
#define ADDRESS_MCP_SLAVE5 0b101

Adafruit_MCP23017 mcp[NUMBER_MCP];
const int ADDRESS[NUMBER_MCP] = {ADDRESS_MCP_MASTER,ADDRESS_MCP_SLAVE1,ADDRESS_MCP_SLAVE2,ADDRESS_MCP_SLAVE3,ADDRESS_MCP_SLAVE4,ADDRESS_MCP_SLAVE5};


void setup() {  
  Serial.begin(SPEED_SERIE);
  
  // Initialisation of MCP
  for(int i=0; i<NUMBER_MCP; i++){
    mcp[i].begin(ADDRESS[i]);                     // Start communication with MCP
    setAllPortToInput(mcp[i],ENABLE_PULLUP);      // Set ALL PORT TO INPUT AND
    activateInterrup(ADDRESS[i]);
  }
 
  // Clear all interupt
  for(int i=0; i<NUMBER_MCP; i++){ 
    clearInterrupt(ADDRESS[i]);                   // Clear all interrupt before launch program
  }
}

void loop() {
  int readGPIO_A = digitalRead(PIN_INTERRUPT_A);
  int readGPIO_B = digitalRead(PIN_INTERRUPT_B);
  int mcpAddress,mcpPort;
  uint8_t readGPIO;
  
  
  if( readGPIO_A == 1){
    readGPIO = mcp[0].readGPIOAB();               // Read GPIO on port A to know witch MCP makes this interrupt
    if( (readGPIO & 0x01) == 0x01 ){
      readSlave(ADDRESS[1],PORT_A);
    }
    if( (readGPIO & 0x02) == 0x02 ){
        readSlave(ADDRESS[1],PORT_B); 
    }
    if( (readGPIO & 0x04) == 0x04 ){
        readSlave(ADDRESS[2],PORT_A); 
    }
    if( (readGPIO & 0x08) == 0x08 ){
        readSlave(ADDRESS[2],PORT_B); 
    }
    if( (readGPIO & 0x10) == 0x10 ){
      readSlave(ADDRESS[3],PORT_A);
    }
    if( (readGPIO & 0x20) == 0x20 ){
        readSlave(ADDRESS[3],PORT_B); 
    }
    if( (readGPIO & 0x40) == 0x40 ){
        readSlave(ADDRESS[4],PORT_A); 
    }
    if( (readGPIO & 0x80) == 0x80 ){
        readSlave(ADDRESS[4],PORT_B); 
    }
  }
  if( readGPIO_B == 1){                            
    readGPIO = mcp[0].readGPIOAB() >> 8;           // Read GPIO on port B to know witch MCP makes this interrupt
     if( (readGPIO & 0x01) == 0x01 ){
      readSlave(ADDRESS[5],PORT_A);
    }
    if( (readGPIO & 0x02) == 0x02 ){
        readSlave(ADDRESS[5],PORT_B); 
    }
    if( (readGPIO & 0x04) == 0x04 ){
        readSlave(ADDRESS[6],PORT_A); 
    }
    if( (readGPIO & 0x08) == 0x08 ){
        readSlave(ADDRESS[6],PORT_B); 
    }
  }
  
  
}

void readSlave(int mcpAddress, int port){
   Serial.print("Interrupt from :");
   Serial.print(mcpAddress);
   Serial.print(" on port: ");
   Serial.println(port);
}


