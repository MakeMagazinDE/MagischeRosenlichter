//-----------------------------------------------------------------------------
//  wlan.cpp
/// @file
///
/// @brief Contains the implementation of the WLan connection management class
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
#include "wlan.h"
#include "AppPreferences.h"
#include "ledeffectmgr.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Maximum wait time for connecting to WLAN (value * 0.5s)
#define CONNECT_TO_WLAN_TIMEOUT     20

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
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
//  Method CWLan
///
/// @brief Constructor of the class CWLan
///
//-----------------------------------------------------------------------------
CWLan::CWLan()
   : m_SystemConnected(false)
{
}

//-----------------------------------------------------------------------------
//  Method ConnectToWLan
///
/// @brief Established a connection to the wlan
///
//-----------------------------------------------------------------------------
void CWLan::ConnectToWLan(void)
{
   String SSID;
   uint8_t timeout =0;

   Serial.println("Connecting to WLAN");

   SSID = AppPreferences::GetSSID();

   if (strcmp(String("No SSID").c_str(), SSID.c_str()))
   {
      WiFi.begin(SSID.c_str(), AppPreferences::GetPassword().c_str());

      Serial.printf("Connecting to %s ", SSID.c_str());

      while (WiFi.status() != WL_CONNECTED && timeout < CONNECT_TO_WLAN_TIMEOUT)
      {
         delay(500);
         Serial.print(".");
         timeout++;
      }

      if (WiFi.status() == WL_CONNECTED)
      {
         Serial.println(" CONNECTED");

         Serial.printf("Subnetmask: %s\r\n", WiFi.subnetMask().toString().c_str());

         m_SystemConnected = true;
      }
      else
      {
         Serial.println("NO CONNECTION TO WLAN!");

         LedEffectMgr.SetSystemStartedEffect(SystemStartType::Error);
      }
   }
   else
   {
      Serial.println("NO CONNECTION TO WLAN!");

      LedEffectMgr.SetSystemStartedEffect(SystemStartType::Error);
   }
}

//-----------------------------------------------------------------------------
//  Method IsConnected
///
/// @brief Gets the information that a WLan connection is exstablished
///
/// @return Flag: Wlan connection is established (true) or not (false)
///
//-----------------------------------------------------------------------------
bool CWLan::IsConnected(void)
{ 
   return m_SystemConnected;
}
