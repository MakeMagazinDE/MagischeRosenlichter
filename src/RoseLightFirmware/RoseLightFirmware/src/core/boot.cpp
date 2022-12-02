//-----------------------------------------------------------------------------
//  boot.cpp
/// @file
///
/// @brief Contains the class to detect the boot mode of the system
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
#include <ArduinoOTA.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "boot.h"
#include "console.h"
#include "ledeffectmgr.h"
#include "soundsensor.h"
#include "roselightota.h"
#include "bluetoothserver.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
#if CONFIG_FREERTOS_UNICORE
 /// Core for running the arduino tasks
 #define ARDUINO_RUNNING_CORE          0
#else
 /// Core for running the arduino tasks
 #define ARDUINO_RUNNING_CORE          1
#endif

/// Boot definition
#define BOOT 0

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// For access to the LED-effect manager instance
extern CLedEffectMgr    LedEffectMgr;
/// For access to the sound manager instance
extern CSoundSensor     SoundSensor;
/// For access to the rose light ota instance
extern CRoseLightOTA    RoseLightOTA;
/// For access to the blue tooth server instance
extern CBluetoothServer BluetoothServer;

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
//  Method CBoot
///
/// @brief Constructor of the class CBoot
///
//-----------------------------------------------------------------------------
CBoot::CBoot(void)
{
   pinMode(BOOT, INPUT);
}
   
//-----------------------------------------------------------------------------
//  Method Elaborate
///
/// @brief Elaborate method of the class
///
//-----------------------------------------------------------------------------
void CBoot::Elaborate(void)
{
   SetName("tBoot");
   SetStackSize(8192);
   SetPriority(2);
   Startup(0);
}

//-----------------------------------------------------------------------------
//  Method TaskFct
///
/// @brief Task function
///
//-----------------------------------------------------------------------------
void CBoot::TaskFct (void)
{
   uint8_t loop = 10;

   Serial.println("Press 'BOOT' for booting in 'Config'-mode ");

   while (loop > 0)
   {
      CTask::Sleep(200);

      if (digitalRead(BOOT) == 0)
      {
         Serial.println("\nBoot in Config-Mode ...");

         BluetoothServer.Elaborate();
         LedEffectMgr.Elaborate();

         LedEffectMgr.SetSystemStartedEffect(SystemStartType::ConfigMode);

         for(;;)
         {
            CTask::Sleep(1000);
         }
      }

      Serial.printf(".");
      loop--;
   }

   Serial.printf("\r\n");

   Serial.println("Boot in 'Application'-Mode ...");

   xTaskCreatePinnedToCore(
      console_task_fct
   ,  "tConsole" 
   ,  8192
   ,  NULL
   ,  2
   ,  NULL 
   ,  ARDUINO_RUNNING_CORE);

   RoseLightOTA.Elaborate();
   
   LedEffectMgr.Elaborate();
   SoundSensor.Elaborate();

   for(;;)
   {
      CTask::Sleep(1000);
   }
}
