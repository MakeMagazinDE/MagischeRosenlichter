//-----------------------------------------------------------------------------
//  roselightota.cpp
/// @file
///
/// @brief Contain the implementation for the OTA mechanism of the system
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <ArduinoOTA.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "roselightota.h"
#include "wlan.h"
#include "udpserver.h"
#include "tcpserver.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// For access to the WLAN management instance
extern CWLan         WLan;
/// For access to the UDP-server instance
extern CUdpServer    UdpServer;
/// For access to the TCP-server instance
extern CTcpServer    TcpServer;

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
//  Method CRoseLightOTA
///
/// @brief Constructor of the class CRoseLightOTA
///
//-----------------------------------------------------------------------------
CRoseLightOTA::CRoseLightOTA()
{
}

//-----------------------------------------------------------------------------
//  Method Elaborate
///
/// @brief Elaborate method of the class
///
//-----------------------------------------------------------------------------
void CRoseLightOTA::Elaborate (void)
{
   SetName("tRlOta");
   SetStackSize(8192);
   SetPriority(2);
   Startup(0);
}

//-----------------------------------------------------------------------------
//  Method InitArduinoOTA
///
/// @brief Initialize the arduino other the air mechanism
///
//-----------------------------------------------------------------------------
void CRoseLightOTA::InitArduinoOTA(void)
{
  Serial.println("Booting");

   WLan.ConnectToWLan();

   if (WLan.IsConnected())
   {
      // Port defaults to 3232
      ArduinoOTA.setPort(3232);

      ArduinoOTA
     .onStart([]()
      {
         String type;
    
         if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
         else // U_SPIFFS
            type = "filesystem";

         // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
         Serial.println("Start updating " + type);
      })
      .onEnd([]()
      {
         Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total)
      {
         Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error)
      {
         Serial.printf("Error[%u]: ", error);
      
         if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
         else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
         else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
         else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
         else if (error == OTA_END_ERROR)
            Serial.println("End Failed");
      });

      ArduinoOTA.begin();

      Serial.println("Ready");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
   }
}

//-----------------------------------------------------------------------------
//  Method TaskFct
///
/// @brief Task function for the OOTA-Update task
///
//-----------------------------------------------------------------------------
void CRoseLightOTA::TaskFct (void)
{
   InitArduinoOTA();

   TcpServer.Elaborate();
   UdpServer.Elaborate();

   for(;;)
   {
      vTaskDelay(100);

      if (WLan.IsConnected())
      {
         ArduinoOTA.handle();
      }
   }
}
