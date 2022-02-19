/*
	Copyright (C) 2022 Victor Chavez
    This file is part of SimpleJ1939.
    SimpleJ1939 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    SimpleJ1939 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with SimpleJ1939.  If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef SIMPLEJ1939_HPP
#define SIMPLEJ1939_HPP

class MCP_CAN;

class SimpleJ1939
{
  public:
    SimpleJ1939(MCP_CAN * pCAN): pCAN(pCAN) {}
    MCP_CAN * pCAN;
    /* Transmit a J1939 Message */
    byte Transmit(long lPGN, byte nPriority, byte nSrcAddr, byte nDestAddr, byte* nData, int nDataLen);
    /* Receive a J1939 Message */
    byte Receive(long* lPGN, byte* nPriority, byte* nSrcAddr, byte *nDestAddr, byte* nData, int* nDataLen);
  private:
    int nWritePointer = 0;
    int nReadPointer = 0;
    static constexpr uint8_t CANMSGBUFFERSIZE = 10;
    struct CANMsg
    {
      unsigned long lID;
      unsigned char pData[8];
      uint8_t nDataLen;
    };
    CANMsg CANMsgBuffer[CANMSGBUFFERSIZE];
    byte canTransmit(long lID, unsigned char* pData, int nDataLen);
    byte canReceive(long* lID, unsigned char* pData, int* nDataLen);
    static bool PeerToPeer(long lPGN);
};

#endif
