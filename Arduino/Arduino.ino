
// Test de base de lecture d'entrée (et activation de la Pull-Up) sur un MCP23017 I/O expander
// Domaine Publique! (code d'origine)

// Connectez la broche #12 du MCP23017 sur Arduino broche Analogique A5 (Horloge I2C, SCL)
// Connectez la broche #13 du MCP23017 sur Arduino broche Analogique A4 (Données I2C, SDA)
// Connectez la broche #15, 16 et 17 du MCP23017 sur DNG (Sélection de l'adresse)
// Connectez la broche #9 du MCP23017 sur 5V (Alimentation)
// Connectez la broche #10 du MCP23017 sur GND (Masse commune)

// Entrée #1 (GPA1, broche 22) est connectée sur bouton puis sur la masse 
// par l'intermédiaire d'une résistance de 330 Ohms

#include "def.h"


// Ethernet
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
EthernetUDP Udp;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
unsigned int PORT_UDP = 8888;
IPAddress ip(10, 1, 1, 177);
IPAddress dest(10,1,1,70); 
unsigned long noFrame = 0x0100;


// MCP23017
#include <Wire.h>
#include "Communication.h"

#include "crc16.h"


const int ADDRESS[] = {ADDRESS_MCP_SLAVE1, ADDRESS_MCP_SLAVE2,ADDRESS_MCP_SLAVE3,ADDRESS_MCP_SLAVE4,ADDRESS_MCP_SLAVE5};
uint8_t value_port_A[NUMBER_MCP - 1];
uint8_t value_port_B[NUMBER_MCP - 1];


void setup() { 

  for (int i = 0; i < ( NUMBER_MCP - 1); i++) {  // Initialise table of value 
    value_port_A[i] = 0x00;
    value_port_B[i] = 0x00;
  }
    
  Wire.begin();                                 // Initialise I2C
  
  configurePcCommunication();
  
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
  sendData(1,12);

  /*if( digitalRead(PIN_INTERRUPT_A) == 0){                        // If interrupt detect on Port A
   
    readGPIO = expanderRead(ADDRESS_MCP_MASTER, PORT_A);          // Read value on GPIOA
    //Serial.println(readGPIO,BIN);
    
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
    
  }
  /*if( digitalRead(PIN_INTERRUPT_B)){                              // If interrupt detect on Port B                            
   
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
  */
  //delay(10);
  delay(2000);
}

void readSlave(int mcpNo, const byte port){
  clearInterrupt(ADDRESS[mcpNo]);
  
   uint8_t readGPIO, mask = 0x01;
   uint8_t compareValue = (port == PORT_A ) ? value_port_A[ mcpNo ] : value_port_B[ mcpNo ] ;   // Get the last value read
   
   readGPIO = expanderRead( ADDRESS[mcpNo] , port);                          // Read GPIO
   
   for(int i = 0; i < 8 ; i++){                                               // For each bit
       if( (readGPIO & mask) == mask && (compareValue & mask) == 0 ){         // Detect press 

            sendData(mcpNo, (port&0x01)<<3 | (readGPIO & mask)  );              // Detect interrupt

            mask = mask << 1; 
       }
   }
   
   if(port == PORT_A)                                                      // Update the last value of GPIO
     value_port_A[ mcpNo - 1 ] = readGPIO;
   else
     value_port_B[ mcpNo - 1 ] = readGPIO;
   
   
}


void sendData(int mcpNo, int input){
  noFrame++;
  if(( noFrame & 0xff) == 0x00)
      noFrame++;

  char answer[] = {noFrame>>8, noFrame & 0xFF, PRESS_CODE, mcpNo, input,0,0,0x00} ;
  
  uint16_t crc16 = calc_crc16((unsigned char*) answer, 5);
  answer[5] = crc16>>8 ; 
  answer[6] = crc16 & 0xFF ; 
  
  Udp.beginPacket(dest, PORT_UDP);
  Udp.write(answer);
  Udp.endPacket();
}

void configurePcCommunication(){
  Ethernet.begin(mac,ip);
  Udp.begin(PORT_UDP);
  
  Serial.begin(9600);
}


