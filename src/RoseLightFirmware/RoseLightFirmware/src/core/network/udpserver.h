//-----------------------------------------------------------------------------
//  udpserver.h
/// @file
///
/// @brief Header for the module udpserver.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __UDPSERVER_H
 #define __UDPSERVER_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <WiFiUDP.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "task.h"
#include "roselightapi.h"

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Class CUdpServer
///
/// @brief Udp server class
///
//-----------------------------------------------------------------------------
class CUdpServer : public CTask
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CUdpServer(void);
   // Elaborate method of the class
   void Elaborate (void);
   // Sends out the message with the id RLA_MSG_ID_FREQ_VALUE_CHANGED
   void SendFrequencyValuesChangedMsg(uint8_t * pFreqValuesPctTable);
   // Sends out the message with the id RLA_MSG_ID_SET_COLOR
   void SendSetColorMsg(uint32_t color);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Task function
   void TaskFct (void);
   // Starts the udp server
   void StartServer(void);
   // Processes the received data
   void ProcessData(uint8_t * pData, uint16_t dataLength);
   // Processes the rla-message with the id RLA_MSG_ID_IS_ANYONE_HERE 
   void ProcessMsg_IsAnyoneHere(void);
   // Processes the rla-message with the id RLA_MSG_ID_STOP_LED_EFFECT
   void ProcessMsg_StopLedEffect(STOP_LED_EFFECT * pData);
   // Processes the rla-message with the id RLA_MSG_ID_FREQ_VALUE_CHANGED
   void ProcessMsg_FreqValueChanged(FREQ_VALUES_CHANGED * pData);
   // Processes the rla-message with the id RLA_MSG_ID_SET_COLOR
   void ProcessMsg_SetColor(SET_COLOR * pData);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Upd device instance
   WiFiUDP m_UdpDevice;
   /// Udp master instance
   WiFiUDP m_UdpMaster;
};

#endif //__UDPSERVER_H
