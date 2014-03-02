
// Test de base de lecture d'entrée (et activation de la Pull-Up) sur un MCP23017 I/O expander
// Domaine Publique! (code d'origine)

// Connectez la broche #12 du MCP23017 sur Arduino broche Analogique A5 (Horloge I2C, SCL)
// Connectez la broche #13 du MCP23017 sur Arduino broche Analogique A4 (Données I2C, SDA)
// Connectez la broche #15, 16 et 17 du MCP23017 sur DNG (Sélection de l'adresse)
// Connectez la broche #9 du MCP23017 sur 5V (Alimentation)
// Connectez la broche #10 du MCP23017 sur GND (Masse commune)

// Entrée #1 (GPA1, broche 22) est connectée sur bouton puis sur la masse 
// par l'intermédiaire d'une résistance de 330 Ohms


#include <Wire.h>
#include "def.h"
#include "Communication.h"

#define PIN_INTERRUPT_A 22
#define PIN_INTERRUPT_B 24

#define PORT_A 0x10
#define PORT_B 0x11


#define SPEED_SERIE 9600


#define NUMBER_MCP 2
#define ADDRESS_MCP_MASTER 0x20
#define ADDRESS_MCP_SLAVE1 0x21
#define ADDRESS_MCP_SLAVE2 0x22
#define ADDRESS_MCP_SLAVE3 0x23
#define ADDRESS_MCP_SLAVE4 0x24
#define ADDRESS_MCP_SLAVE5 0x25

const int ADDRESS[] = {ADDRESS_MCP_SLAVE1, ADDRESS_MCP_SLAVE2,ADDRESS_MCP_SLAVE3,ADDRESS_MCP_SLAVE4,ADDRESS_MCP_SLAVE5};
uint8_t value_port_A[NUMBER_MCP - 1];
uint8_t value_port_B[NUMBER_MCP - 1];


void setup() { 
  
  for (int i = 0; i < ( NUMBER_MCP - 1); i++) {  // Initialise table of value 
    value_port_A[i] = 0x00;
    value_port_B[i] = 0x00;
  }  
    
  Wire.begin ();                                 // Initialise I2C
  
  
  Serial.begin(SPEED_SERIE);                     // Initialise Serial
  
  
  for(int i=0; i<NUMBER_MCP; i++){               // Configuration Slave
      configureSlave(ADDRESS[i]);
  }
  configureMaster(ADDRESS_MCP_MASTER);           // Configure Master
 
  for(int i=0; i<NUMBER_MCP; i++){ 
    clearInterrupt(ADDRESS[i]);                  // Clear all interrupt before launch program
  }
  clearInterrupt(ADDRESS_MCP_MASTER);
}

void loop() {
  uint8_t readGPIO;
  
  if( 1 == 1){ //digitalRead(PIN_INTERRUPT_A) == 0){                         // If interrupt detect on Port A
   
    readGPIO = expanderRead(ADDRESS_MCP_MASTER, PORT_A);          // Read value on GPIOA
    
    if( (readGPIO & 0x01) == 0 ){
        readSlave(0,PORT_A);
    }
    if( (readGPIO & 0x02) == 0 ){
        readSlave(0,PORT_B); 
    }
   /* if( (readGPIO & 0x04) == 0x04 ){
        readSlave(1,PORT_A); 
    }
    if( (readGPIO & 0x08) == 0x08 ){
        readSlave(1,PORT_B); 
    }
    if( (readGPIO & 0x10) == 0x10 ){
      readSlave(2, PORT_A);
    }
    if( (readGPIO & 0x20) == 0x20 ){
        readSlave(2, PORT_B); 
    }
    if( (readGPIO & 0x40) == 0x40 ){
        readSlave(3, PORT_A); 
    }
    if( (readGPIO & 0x80) == 0x80 ){
        readSlave(3, PORT_B); 
    }
    */
  }
  if( digitalRead(PIN_INTERRUPT_B) == 91){                        // If interrupt detect on Port B                            
   
    readGPIO = expanderRead(ADDRESS_MCP_MASTER, PORT_B);          // Read value on GPIOB
    
    Serial.println(readGPIO,BIN);
    
     if( (readGPIO & 0x01) == 0x01 ){
        readSlave(4, PORT_A);
    }
    if( (readGPIO & 0x02) == 0x02 ){
        readSlave(4, PORT_B); 
    }
    if( (readGPIO & 0x04) == 0x04 ){
        readSlave(5, PORT_A); 
    }
    if( (readGPIO & 0x08) == 0x08 ){
        readSlave(5, PORT_B); 
    }
  }
  delay(10);
}

void readSlave(int mcpNo, const byte port){
   
   uint8_t readGPIO, mask = 0x01;
   uint8_t compareValue = (port == PORT_A ) ? value_port_A[ mcpNo ] : value_port_B[ mcpNo ] ;   // Get the last value read
   
   // Display debug
   Serial.print("Interrupt from :");
   Serial.print( mcpNo );
   Serial.print(" on port: ");
   Serial.println( port );
   
   readGPIO = expanderRead( ADDRESS[mcpNo] , port);                          // Read GPIO
   
   for(int i = 0; i < 8 ; i++){                                             // For each bit
       if( (readGPIO & mask) == mask && (compareValue & mask) == 0 ){       // Detect change 
         Serial.print(i);
         Serial.print(",");
       }
      mask = mask << 1; 
   }
   Serial.println("");
   
   if(port == PORT_A)                                                      // Update the last value of GPIO
     value_port_A[ mcpNo - 1 ] = readGPIO;
   else
     value_port_B[ mcpNo - 1 ] = readGPIO;
  
   
   clearInterrupt(ADDRESS[mcpNo]); 
}


