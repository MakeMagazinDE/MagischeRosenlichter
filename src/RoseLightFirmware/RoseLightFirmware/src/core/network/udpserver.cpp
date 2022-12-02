//-----------------------------------------------------------------------------
//  udpserver.cpp
/// @file
///
/// @brief Contains the implementation of the udp server class
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
#include <WiFi.h>
#include <string.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "udpserver.h"
#include "wlan.h"
#include "apppreferences.h"
#include "tcpserver.h"
#include "ledeffectmgr.h"
#include "ledeffectdefs.h"
#include "version.h"
#include "soundsensor.h"

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
CWLan                WLan;
/// For access to the tcp-server instance
CTcpServer           TcpServer;
/// For access to the LED-effect manager instance
extern CLedEffectMgr LedEffectMgr;
/// For access to the sound sensor instance
extern CSoundSensor  SoundSensor;

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
//  Method CUdpServer
///
/// @brief Constructor of the class
///
//-----------------------------------------------------------------------------
CUdpServer::CUdpServer(void)
{
}

//-----------------------------------------------------------------------------
//  Method TaskFct
///
/// @brief Task function
///
//-----------------------------------------------------------------------------
void CUdpServer::TaskFct(void)
{
   int packetSize;
   char buffer[100];

   StartServer();

   for(;;)
   {
      vTaskDelay(10);

      if (!WLan.IsConnected())
      {
         vTaskDelay(10000);
         continue;
      }

      m_UdpDevice.beginPacket(WiFi.subnetMask(), UDP_DEVICE_REMOTE_PORT);

      packetSize = m_UdpDevice.parsePacket();

      if (packetSize)
      {
#if 0
         Serial.printf("Received packet size is: %i bytes\r\n", packetSize);
         Serial.printf("Packet received from: %s\r\n", m_UdpDevice.remoteIP().toString().c_str());
         Serial.printf("Port address is: %i\r\n", m_UdpDevice.remotePort());
#endif

         if (packetSize > 0)
         {
            int len = m_UdpDevice.read(buffer, 100);

            if (len > 0)
            {
               ProcessData((uint8_t *)buffer, len);
            }
         }
      }

      m_UdpDevice.endPacket();
   }

   WiFi.disconnect(); 
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
//-----------------------------------------------------------------------------
void CUdpServer::ProcessData(uint8_t * pData, uint16_t dataLength)
{
   uint16_t msgId;

   if (2 <= dataLength)
   {
      msgId = pData[0];
      msgId |= pData[1] << 8;

      if (RLA_MSG_ID_FREQ_VALUE_CHANGED != msgId)
      {
         Serial.printf("Receive command: 0x%04x\r\n", msgId);
      }

      switch (msgId)
      {
         case RLA_MSG_ID_IS_ANYONE_HERE:
            ProcessMsg_IsAnyoneHere();
            break;

         case RLA_MSG_ID_STOP_LED_EFFECT:
            ProcessMsg_StopLedEffect((STOP_LED_EFFECT * )&pData[2]);
            break;

         case RLA_MSG_ID_FREQ_VALUE_CHANGED:
            ProcessMsg_FreqValueChanged((FREQ_VALUES_CHANGED * )&pData[2]);
            break;

         case RLA_MSG_ID_SET_COLOR:
            ProcessMsg_SetColor((SET_COLOR * )&pData[2]);
            break;

         default:
            break;
      }
   }
}

//-----------------------------------------------------------------------------
//  Method ProcessMsg_IsAnyoneHere
///
/// @brief Processes the rla-message with the id RLA_MSG_ID_IS_ANYONE_HERE
///
//-----------------------------------------------------------------------------
void CUdpServer::ProcessMsg_IsAnyoneHere(void)
{
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

   m_UdpDevice.write(buffer, RLA_SIZE_OF_MSG_ID + sizeof(BASIC_DEVICE_INFO));

   // Save the last host for tcp connection
   AppPreferences::SetLastHost(m_UdpDevice.remoteIP().toString().c_str());
}

//-----------------------------------------------------------------------------
//  Method ProcessMsg_StopLedEffect
///
/// @brief Processes the rla-message with the id RLA_MSG_ID_STOP_LED_EFFECT
///
//-----------------------------------------------------------------------------
void CUdpServer::ProcessMsg_StopLedEffect(STOP_LED_EFFECT * pData)
{
   LedEffectMgr.StopEffect((StopEffectSequence)pData->stopEffectSequence);
   SoundSensor.DeactivateSoundSensor();
}

//-----------------------------------------------------------------------------
//  Method ProcessMsg_FreqValueChanged
///
/// @brief Processes the rla-message with the id RLA_MSG_ID_FREQ_VALUE_CHANGED
///
//-----------------------------------------------------------------------------
void CUdpServer::ProcessMsg_FreqValueChanged(FREQ_VALUES_CHANGED * pData)
{
   // if we are the master ignore the message, we have send out the message
   if (2 == AppPreferences::GetType())
      return;

   uint8_t frequencyValues[7] = 
      { pData->Freq63HzValuePct,
        pData->Freq160HzValuePct,
        pData->Freq400HzValuePct,
        pData->Freq1000HzValuePct,
        pData->Freq2500HzValuePct,
        pData->Freq6250HzValuePct,
        pData->Freq16000HzValuePct
      };

   LedEffectMgr.SetFrequencyValues(frequencyValues);
}

//-----------------------------------------------------------------------------
//  Method ProcessMsg_SetColor
///
/// @brief Processes the rla-message with the id RLA_MSG_ID_SET_COLOR
///
//-----------------------------------------------------------------------------
void CUdpServer::ProcessMsg_SetColor(SET_COLOR * pData)
{
   // if we are the master ignore the message, we have send out the message
   if (2 == AppPreferences::GetType())
      return;

   LedEffectMgr.SetColorToEffect(pData->Color);
}

//-----------------------------------------------------------------------------
//  Method Elaborate
///
/// @brief Elaborate method of the class
///
//-----------------------------------------------------------------------------
void CUdpServer::Elaborate (void)
{
   Serial.println("CUdpServer::Elaborate()");

   if (WLan.IsConnected())
   {
      SetName("tUdpServer");
      SetStackSize(8192);
      SetPriority(2);
      Startup(0);
   }
   else
   {
      Serial.println("WLan not connected! Udp server not started!");
   }
}

//-----------------------------------------------------------------------------
//  Method StartServer
///
/// @brief Starts the udp server
///
//-----------------------------------------------------------------------------
void CUdpServer::StartServer(void)
{
   Serial.println("Start udp server ...");

   if (WLan.IsConnected())
   {
      m_UdpDevice.begin(UDP_DEVICE_LOCAL_PORT);
      m_UdpMaster.begin(UDP_MASTER_LOCAL_PORT);

      LedEffectMgr.SetSystemStartedEffect(SystemStartType::Normal);
   }
   else
   {
      Serial.println("Udp server not started!");
   }
}

//-----------------------------------------------------------------------------
//  Method SendFrequencyValuesChangedMsg
///
/// @brief Sends out the message with the id RLA_MSG_ID_FREQ_VALUE_CHANGED
///
/// @param[in] pFrequencyValues - Frequency values
///
//-----------------------------------------------------------------------------
void CUdpServer::SendFrequencyValuesChangedMsg(uint8_t * pFrequencyValues)
{
   uint8_t buffer[RLA_SIZE_OF_MSG_ID + sizeof(FREQ_VALUES_CHANGED)];

   memset(buffer, 0, RLA_SIZE_OF_MSG_ID + sizeof(FREQ_VALUES_CHANGED));

   // Add message id
   buffer[0] = RLA_MSG_ID_FREQ_VALUE_CHANGED & 0xFF;
   buffer[1] = (RLA_MSG_ID_FREQ_VALUE_CHANGED >> 8) & 0xFF;

   memcpy(&buffer[2], pFrequencyValues, 7);

   m_UdpMaster.beginPacket(IPAddress(255,255,255,255), UDP_MASTER_REMOTE_PORT);
   m_UdpMaster.write(buffer, RLA_SIZE_OF_MSG_ID + sizeof(FREQ_VALUES_CHANGED));
   m_UdpMaster.endPacket();
}

//-----------------------------------------------------------------------------
//  Method SendSetColorMsg
///
/// @brief Sends out the message with the id RLA_MSG_ID_SET_COLOR
///
/// @param[in] color - Color value
///
//-----------------------------------------------------------------------------
void CUdpServer::SendSetColorMsg(uint32_t color)
{
   uint8_t buffer[RLA_SIZE_OF_MSG_ID + sizeof(SET_COLOR)];

   memset(buffer, 0, RLA_SIZE_OF_MSG_ID + sizeof(SET_COLOR));

   // Add message id
   buffer[0] = RLA_MSG_ID_SET_COLOR & 0xFF;
   buffer[1] = (RLA_MSG_ID_SET_COLOR >> 8) & 0xFF;

   buffer[2] = color & 0xFF;
   buffer[3] = (color >> 8) & 0xFF;
   buffer[4] = (color >> 16) & 0xFF;
   buffer[5] = (color >> 24) & 0xFF;

   m_UdpMaster.beginPacket(IPAddress(255,255,255,255), UDP_MASTER_REMOTE_PORT);
   m_UdpMaster.write(buffer, RLA_SIZE_OF_MSG_ID + sizeof(SET_COLOR));
   m_UdpMaster.endPacket();
}
