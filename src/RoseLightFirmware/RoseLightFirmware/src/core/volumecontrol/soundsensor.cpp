//-----------------------------------------------------------------------------
//  soundsensor.cpp
/// @file
///
/// @brief Contains the implementation of the sound sensor management class
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

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "soundsensor.h"
#include "ledeffectmgr.h"
#include "udpserver.h"
#include "AppPreferences.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Maximum ad-converter value
#define MAX_VALUE       2800
/// Output of the MSGEQ7 chip
#define MSGEQ7_OUT      34
/// Strobe pin of MSGEQ7 chip
#define MSGEQ7_STROBE   16
/// Reset pin of MSGEQ7 chip
#define MSGEQ7_RESET    17

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of private functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global members of the module
//-----------------------------------------------------------------------------
/// For access to the sound sensor management instance
extern CSoundSensor     SoundSensor;
/// For access to the LED-effect manager instance
extern CLedEffectMgr    LedEffectMgr;
/// For access to the UDP-server instance
extern CUdpServer       UdpServer;

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Method CSoundSensor
///
/// @brief Constructor of the class CSoundSensor
///
//-----------------------------------------------------------------------------
CSoundSensor::CSoundSensor(void)
   : m_SoundSensorActive(false)
{
}

//-----------------------------------------------------------------------------
//  Method ActivateSoundSensor
///
/// @brief Activates the sound sensor
///
//-----------------------------------------------------------------------------
void CSoundSensor::ActivateSoundSensor(void)
{
   m_SoundSensorActive = true;

   xSemaphoreGive(m_JobSema);
}

//-----------------------------------------------------------------------------
//  Method DeactivateSoundSensor
///
/// @brief Deactivates the sound sensor
///
//-----------------------------------------------------------------------------
void CSoundSensor::DeactivateSoundSensor(void)
{
   m_SoundSensorActive = false;
}

//-----------------------------------------------------------------------------
//  Method TaskFct
///
/// @brief Task function of the sound sensor manager
///
//-----------------------------------------------------------------------------
void CSoundSensor::TaskFct(void)
{
#if 0
   int msgeq7_values[7] = { 0,0,0,0,0,0,0 };
#endif

   uint8_t msgeq7_values_pct[7] = { 0,0,0,0,0,0,0 };
   int msgeq7_value;
   TickType_t xLastWakeTime;
   const TickType_t xFrequency = 50;

   for(;;)
   {
      xSemaphoreTake(m_JobSema, portMAX_DELAY);

      Serial.printf("Activate sound sensor and FFT ...");

      m_SoundSensorActive = true;

      xLastWakeTime = xTaskGetTickCount();

      while (m_SoundSensorActive)
      {
         vTaskDelayUntil( &xLastWakeTime, xFrequency );

         digitalWrite(2, HIGH);

         digitalWrite(MSGEQ7_RESET, HIGH);
         digitalWrite(MSGEQ7_RESET, LOW);
      
         for(int band=0; band <7; band++)
         {
            digitalWrite(MSGEQ7_STROBE,LOW); 
      
            delayMicroseconds(30); 

            msgeq7_value = analogRead(MSGEQ7_OUT);
#if 0
            msgeq7_values[band] = msgeq7_value;
#endif

            msgeq7_values_pct[band] = (msgeq7_value *100)/4095;

            digitalWrite(MSGEQ7_STROBE,HIGH); 
         }

         LedEffectMgr.SetFrequencyValues(msgeq7_values_pct);

         UdpServer.SendFrequencyValuesChangedMsg(msgeq7_values_pct);

#if 0
         Serial.printf("Values: %i, %i, %i, %i, %i, %i, %i\r\n", 
            msgeq7_values[0], msgeq7_values[1], msgeq7_values[2], msgeq7_values[3], msgeq7_values[4], msgeq7_values[5], msgeq7_values[6]);         
#endif

         digitalWrite(2, LOW);
      }

      Serial.printf("Sound sensor deactivated!");
   }
}

//-----------------------------------------------------------------------------
//  Method Elaborate
///
/// @brief Elaborate method of the sound sensor manager
///
//-----------------------------------------------------------------------------
void CSoundSensor::Elaborate (void)
{
   if (2 != AppPreferences::GetType())
      return;

   Serial.println("CSoundSensor::Elaborate()");

   m_JobSema = xSemaphoreCreateBinary();

   SetName("tSndSensor");
   SetStackSize(8192);
   SetPriority(2);
   Startup(0);

   pinMode(MSGEQ7_RESET, OUTPUT); // reset
   pinMode(MSGEQ7_STROBE, OUTPUT); // strobe
   digitalWrite(MSGEQ7_RESET,LOW); // reset low
   digitalWrite(MSGEQ7_STROBE,HIGH); //pin 5 is RESET on the shield
}
