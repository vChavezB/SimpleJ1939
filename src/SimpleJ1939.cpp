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
#include <mcp_can.h>
#include "SimpleJ1939.hpp"

// ------------------------------------------------------------------------
// Transmit CAN message
// ------------------------------------------------------------------------
byte SimpleJ1939::canTransmit(long lID, unsigned char* pData, int nDataLen)
{
  if (pCAN->sendMsgBuf(lID, CAN_EXTID, nDataLen, pData) == 0)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

// ------------------------------------------------------------------------
// Receive CAN message
// ------------------------------------------------------------------------
byte SimpleJ1939::canReceive(long* lID, unsigned char* pData, int* nDataLen)
{
  // Declarations
  byte nCnt;

  // In case there is a message, put it into the buffer
  while (pCAN->checkReceive() == CAN_MSGAVAIL)
  {
    // Read the message buffer

    pCAN->readMsgBuf(&CANMsgBuffer[nWritePointer].lID,
                     &CANMsgBuffer[nWritePointer].nDataLen,
                     &CANMsgBuffer[nWritePointer].pData[0]);

    if (++nWritePointer == CANMSGBUFFERSIZE)
    {
      nWritePointer = 0;
    }

  }// end while

  // Check ring buffer for a message
  if (nReadPointer != nWritePointer)
  {
    // Read the next message buffer entry
    *nDataLen = CANMsgBuffer[nReadPointer].nDataLen;
    *lID = CANMsgBuffer[nReadPointer].lID;

    for (int nIdx = 0; nIdx < *nDataLen; nIdx++)
    {
      pData[nIdx] = CANMsgBuffer[nReadPointer].pData[nIdx];
    }

    if (++nReadPointer == CANMSGBUFFERSIZE)
    {
      nReadPointer = 0;
    }

    return 0;

  }// end if
  else 
  {
	  return 1;
  }

}// end canReceive

// ------------------------------------------------------------------------
// J1939 Check Peer-to-Peer
// ------------------------------------------------------------------------
bool SimpleJ1939::PeerToPeer(long lPGN)
{
  // Check the PGN
  if (lPGN > 0 && lPGN <= 0xEFFF)
  {
    return true;
  }
  if (lPGN > 0x10000 && lPGN <= 0x1EFFF)
  {
    return true;
  }

  return false;

}// end PeerToPeer

// ------------------------------------------------------------------------
// J1939 Transmit
// ------------------------------------------------------------------------
byte SimpleJ1939::Transmit(long lPGN, byte nPriority, byte nSrcAddr, byte nDestAddr, byte* nData, int nDataLen)
{
  // Declarations
  long lID = static_cast<long>(nPriority)<< 26 | static_cast<long>(lPGN << 8) | static_cast<long>(nSrcAddr);

  // If PGN represents a peer-to-peer, add destination address to the ID
  if (PeerToPeer(lPGN) == true)
  {
    lID = lID & 0xFFFF00FF;
    lID = lID | (static_cast<long>(nDestAddr) << 8);

  }// end if

  // Transmit the message
  return canTransmit(lID, nData, nDataLen);

}// end j1939Transmit

// ------------------------------------------------------------------------
// J1939 Receive
// ------------------------------------------------------------------------
byte SimpleJ1939::Receive(long* lPGN, byte* nPriority, byte* nSrcAddr, byte *nDestAddr, byte* nData, int* nDataLen)
{
  // Declarations
  byte nRetCode = 1;
  long lID;

  // Default settings
  *nSrcAddr = 255;
  *nDestAddr = 255;

  if (canReceive(&lID, nData, nDataLen) == 0)
  {
    long lPriority = lID & 0x1C000000;
    *nPriority = static_cast<int>(lPriority >> 26);

    *lPGN = lID & 0x00FFFF00;
    *lPGN = *lPGN >> 8;

    lID = lID & 0x000000FF;
    *nSrcAddr = static_cast<int>(lID);

    if (PeerToPeer(*lPGN) == true)
    {
      *nDestAddr = static_cast<int>(*lPGN & 0xFF);
      *lPGN = *lPGN & 0x01FF00;
    }

    nRetCode = 0;

  }// end if
  return nRetCode;
}// end j1939Receive
