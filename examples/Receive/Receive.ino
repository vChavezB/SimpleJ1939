#include "SimpleJ1939.hpp"
#include <mcp_can.h>
static unsigned constexpr MCP_CS = 5; //Chip Select Pin

MCP_CAN CAN0(MCP_CS);
SimpleJ1939 J1939(&CAN0);

void setup()
{
  // Set the serial interface baud rate
  Serial.begin(115200);
  if(CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ) == CAN_OK)
  {
    CAN0.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515 sends acks to received data.
  }
  else
  {
    Serial.print("CAN Init Failed.\n\r");
    while(1);
  }
}

void loop()
{
  // Declarations
  byte nPriority;
  byte nSrcAddr;
  byte nDestAddr;
  byte nData[8];
  int nDataLen;
  long lPGN;

  char sString[80];

  // Check for received J1939 messages
  if (J1939.Receive(&lPGN, &nPriority, &nSrcAddr, &nDestAddr, nData, &nDataLen) == 0)
  {
    sprintf(sString, "PGN: 0x%X Src: 0x%X Dest: 0x%X ", (int)lPGN, nSrcAddr, nDestAddr);
    Serial.print(sString);
    if (nDataLen == 0 )
    {
      Serial.print("No Data.\n\r");
    }
    else
    {
      Serial.print("Data: ");
      for (int nIndex = 0; nIndex < nDataLen; nIndex++)
      {
        sprintf(sString, "0x%X ", nData[nIndex]);
        Serial.print(sString);

      }
      Serial.print("\n\r");
    }
  }
}