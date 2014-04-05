#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

uint16_t calc_crc16(unsigned char* data, unsigned short length)
{
  uint16_t CRC = 0x0000;
  uint16_t A = 0x0000, B = 0x0000;
  uint8_t index = 0, Counter = 0;

  for( index = 0; index < length; index++ )
  {
    uint8_t _byte = data[index];

    A = CRC / 256;
    A = A ^ _byte;
    A = A * 256;

    B = CRC & 0xFF;
    CRC = A | B;

    for( Counter = 0; Counter < 8; Counter++ ){
      if( (CRC & 0x8000) > 0)
      {
        CRC = CRC * 2;
        CRC = CRC ^ 0x8005;
      }
      else
      {
        CRC = CRC * 2;
      }
    }
  }
  return CRC;
}


