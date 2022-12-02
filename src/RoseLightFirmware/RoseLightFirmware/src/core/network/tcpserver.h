//-----------------------------------------------------------------------------
//  tcpserver.h
/// @file
///
/// @brief Header for the module tcpserver.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __TCPSERVER_H
 #define __TCPSERVER_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <WiFi.h>

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
//  Class CTcpServer
///
/// @brief Tcp server class
///
//-----------------------------------------------------------------------------
class CTcpServer : public CTask
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CTcpServer(void);
   // Elaborate method of the class
   void Elaborate (void);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Task function
   void TaskFct (void);
   // Starts the tcp server
   void StartServer(void);
   // Processes the received data 
   void ProcessData(uint8_t * pData, uint16_t dataLength, WiFiClient * pClient);
   // Processes the rla-message with the id RLA_MSG_ID_DEVICE_NAME
   void ProcessMsg_DeviceName(DEVICE_NAME * pDeviceName);
   // Processes the rla-message with the id RLA_MSG_ID_REQ_BASIC_DEVICE_INFO
   void ProcessMsg_ReqBasicDeviceInfo(WiFiClient * pClient);
   // Processes the rla-message with the id RLA_MSG_ID_START_LED_EFFECT
   void ProcessMsg_StartLedEffect(START_LED_EFFECT * pData);
   // Processes the rla-message with the id RLA_MSG_ID_STOP_LED_EFFECT
   void ProcessMsg_StopLedEffect(STOP_LED_EFFECT * pData);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Flag: System is connected (true) or not (false)
   bool m_SystemConnected; 
   /// WiFi server instance
   WiFiServer * m_pTcp;
};

#endif //__TCPSERVER_H
