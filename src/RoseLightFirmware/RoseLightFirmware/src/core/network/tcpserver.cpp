//-----------------------------------------------------------------------------
//  tcpserver.cpp
/// @file
///
/// @brief Contains the implementation of the tcp server class
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <Arduino.h>
#include <string.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "tcpserver.h"
#include "AppPreferences.h"
#include "wlan.h"
#include "ledeffectmgr.h"
#include "ledeffectdefs.h"
#include "version.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// For access to the WLan instance
extern CWLan         WLan;
/// For access to the LED-effect manager instance
extern CLedEffectMgr LedEffectMgr;

//-----------------------------------------------------------------------------
// Prototypes of private functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global members of the module
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Method CTcpServer
///
/// @brief Constructor of the class
///
//-----------------------------------------------------------------------------
CTcpServer::CTcpServer(void)
   : m_SystemConnected(false)
   , m_pTcp(NULL)
{
}

//-----------------------------------------------------------------------------
//  Method Elaborate
///
/// @brief Elaborate method of the class
///
//-----------------------------------------------------------------------------
void CTcpServer::Elaborate (void)
{
   Serial.println("CTcpServer::Elaborate()");

   if (WLan.IsConnected())
   {
      m_pTcp = new WiFiServer(6223);

      if (NULL != m_pTcp)
      {
         SetName("tTcpServer");
         SetStackSize(8192);
         SetPriority(2);
         Startup(0);
      }
      else
      {
         Serial.println("Can't create WiFiServer object! Tcp server not started!");
      }
   }
   else
   {
      Serial.println("WLan not connected! Tcp server not started!");
   }
}

//-----------------------------------------------------------------------------
//  Method TaskFct
///
/// @brief Task function
///
//-----------------------------------------------------------------------------
void CTcpServer::TaskFct (void)
{
   uint8_t buffer[100];

   StartServer();

   for(;;)
   {
      vTaskDelay(100);

      if (!WLan.IsConnected())
      {
         vTaskDelay(10000);
         continue;
      }

      //String lastHost = AppPreferences::GetLastHost();

      WiFiClient client = m_pTcp->available();

      if (client)
      {         
         while (client.connected())
         {
            while (client.available() > 0) 
            {
               Serial.println("Client available!");

               int len = client.read(buffer, 100);

               Serial.printf("Packet received from: %s\r\n", client.remoteIP().toString().c_str());
               Serial.printf("Length: %i bytes\r\n", len);

               if (len > 0)
               {
                  ProcessData((uint8_t *)buffer, len, &client);
               }
            }
 
            vTaskDelay(10);
         }
      }

      client.stop();
   }
}

//-----------------------------------------------------------------------------
//  Method StartServer
///
/// @brief Starts the tcp server
///
//-----------------------------------------------------------------------------
void CTcpServer::StartServer(void)
{
   Serial.println("Start tcp server ...");

#if 0
   Serial.print("Local ip address is: ");
   Serial.println(WiFi.localIP());
#endif

   m_pTcp->begin();
}

//-----------------------------------------------------------------------------
//  Method ProcessData
///
/// @brief Processes the received data
///
/// @param[in] pData - Received data
///
/// @param[in] dataLength - Data length
///
/// @param[in] pClient - Network client
///
//-----------------------------------------------------------------------------
void CTcpServer::ProcessData(uint8_t * pData, uint16_t dataLength, WiFiClient * pClient)
{
   uint16_t msgId;

   Serial.println("ProcessData");

   if (2 <= dataLength)
   {
      msgId = pData[0];
      msgId |= pData[1] << 8;

      Serial.printf("Receive command: 0x%04x\r\n", msgId);

      switch (msgId)
      {
         case RLA_MSG_ID_DEVICE_NAME:
            ProcessMsg_DeviceName((DEVICE_NAME *)&pData[2]);
            break;

         case RLA_MSG_ID_REQ_BASIC_DEVICE_INFO:
            ProcessMsg_ReqBasicDeviceInfo(pClient);
            break;

         case RLA_MSG_ID_START_LED_EFFECT:
            ProcessMsg_StartLedEffect((START_LED_EFFECT * )&pData[2]);
            break;

         case RLA_MSG_ID_STOP_LED_EFFECT:
            ProcessMsg_StopLedEffect((STOP_LED_EFFECT *)&pData[2]);
            break;

         default:
            break;
      }
   }
}

//-----------------------------------------------------------------------------
//  Method ProcessMsg_DeviceName
///
/// @brief Processes the rla-message with the id RLA_MSG_ID_DEVICE_NAME
///
/// @param[in] pDeviceName - Message data
///
//-----------------------------------------------------------------------------
void CTcpServer::ProcessMsg_DeviceName(DEVICE_NAME * pDeviceName)
{
   if (NULL == pDeviceName)
      return;

   // Save the last host for tcp connection
   AppPreferences::SetName((char *)pDeviceName->name);
}

//-----------------------------------------------------------------------------
//  Method ProcessMsg_ReqBasicDeviceInfo
///
/// @brief Processes the rla-message with the id RLA_MSG_ID_REQ_BASIC_DEVICE_INFO
///
/// @param[in] pClient - Message data
///
//-----------------------------------------------------------------------------
void CTcpServer::ProcessMsg_ReqBasicDeviceInfo(WiFiClient * pClient)
{
   if (NULL == pClient)
      return;

   uint8_t buffer[RLA_SIZE_OF_MSG_ID + sizeof(BASIC_DEVICE_INFO)];

   memset(buffer, 0, RLA_SIZE_OF_MSG_ID + sizeof(BASIC_DEVICE_INFO));

   // Add message id
   buffer[0] = RLA_MSG_ID_BASIC_DEVICE_INFO & 0xFF;
   buffer[1] = (RLA_MSG_ID_BASIC_DEVICE_INFO >> 8) & 0xFF;
   
   buffer[2] = AppPreferences::GetType();

   strcpy((char *)&buffer[3], AppPreferences::GetName().c_str());

   uint64_t chipId = ESP.getEfuseMac();

   memcpy(&buffer[23], &chipId, sizeof(uint64_t));

   buffer[31] = AppPreferences::GetFlags();

   buffer[32] = VERSION;
   buffer[33] = REVISION;
   buffer[34] = SUBREVISION;

   pClient->write(buffer, RLA_SIZE_OF_MSG_ID + sizeof(BASIC_DEVICE_INFO));
}

//-----------------------------------------------------------------------------
//  Method ProcessMsg_StartLedEffect
///
/// @brief Processes the rla-message with the id RLA_MSG_ID_START_LED_EFFECT
///
/// @param[in] pData - Message data
///
//-----------------------------------------------------------------------------
void CTcpServer::ProcessMsg_StartLedEffect(START_LED_EFFECT * pData)
{
   LedEffectMgr.StopEffect(StopEffectSequence::HardSwitchOff);

   LedEffectMgr.SetEffectParameter(pData->effectParams);

   LedEffectMgr.StartEffect((LedEffectId)pData->ledEffectId);
}

//-----------------------------------------------------------------------------
//  Method ProcessMsg_StopLedEffect
///
/// @brief Processes the rla-message with the id RLA_MSG_ID_STOP_LED_EFFECT
///
/// @param[in] pData - Message data
///
//-----------------------------------------------------------------------------
void CTcpServer::ProcessMsg_StopLedEffect(STOP_LED_EFFECT * pData)
{
   LedEffectMgr.StopEffect((StopEffectSequence)pData->stopEffectSequence);
}
