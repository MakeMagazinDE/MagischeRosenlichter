//-----------------------------------------------------------------------------
//  nclient.cpp
/// @file
///
/// @brief Contains the implementation of the network client task
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
#include <WiFiUDP.h>
#include <string.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "nclient.h"

#include "AppPreferences.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of private functions
//-----------------------------------------------------------------------------
/// Elaborate function of the network client
void nclient_Elaborate(void);

//-----------------------------------------------------------------------------
// Global members of the module
//-----------------------------------------------------------------------------
/// Udp instance
WiFiUDP udp;
/// Flag: System is connected or not
bool systemConnected = false;

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Function nclient_task_fct
///
/// @brief Network client task function
///
//-----------------------------------------------------------------------------
void nclient_task_fct( void *pvParameters )
{
   int packetSize;
   char buffer[100];

   nclient_Elaborate();

   for(;;)
   {
      vTaskDelay(10);

      if (!systemConnected)
      {
         vTaskDelay(10000);
         continue;
      }

      udp.beginPacket(WiFi.subnetMask(), 5000);

      packetSize = udp.parsePacket();

      if (packetSize)
      {
         Serial.printf("Received packet size is: %i bytes\r\n", packetSize);
         Serial.printf("Packet received from: %s\r\n", udp.remoteIP().toString().c_str());
         Serial.printf("Port address is: %i\r\n", udp.remotePort());

         if (packetSize > 0)
         {
            int len = udp.read(buffer, 100);

            if (len > 0)
            {
               Serial.println(&buffer[0]);
            }

            vTaskDelay(200);

            udp.printf("Hello from the server!");

            vTaskDelay(200);

            udp.printf("Hello from the server!");

            vTaskDelay(200);

            udp.printf("Hello from the server!");

            vTaskDelay(200);

            udp.printf("Hello from the server!");

            vTaskDelay(200);

            udp.printf("Hello from the server!");

            vTaskDelay(1000);
         }
      }

      udp.endPacket();
   }

   WiFi.disconnect();
}

//-----------------------------------------------------------------------------
//  Function nclient_Elaborate
///
/// @brief Network client elaborate function
///
//-----------------------------------------------------------------------------
void nclient_Elaborate(void)
{
   String SSID;

   SSID = AppPreferences::GetSSID();

   if (strcmp(String("No SSID").c_str(), SSID.c_str()))
   {
      WiFi.begin(SSID.c_str(), AppPreferences::GetPassword().c_str());

      Serial.printf("Connecting to %s ", SSID.c_str());

      while (WiFi.status() != WL_CONNECTED)
      {
         delay(500);
         Serial.print(".");
      }

      Serial.println(" CONNECTED");

      Serial.printf("Subnetmask: %s\r\n", WiFi.subnetMask().toString().c_str());

      udp.begin(5000);

      systemConnected = true;
   }
   else
   {
      Serial.println("NO CONNECTED TO WLAN!");
   }   
}
