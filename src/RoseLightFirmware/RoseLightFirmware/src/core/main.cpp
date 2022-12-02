//-----------------------------------------------------------------------------
//  main.cpp
/// @file
///
/// @brief Main module of the application
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
#include "console.h"
#include "nclient.h"

#include "AppPreferences.h"

#include "ledeffectmgr.h"
#include "udpserver.h"
#include "soundsensor.h"
#include "roselightota.h"
#include "bluetoothserver.h"

#include "effectmulticolor.h"
#include "effectsinglecolor.h"
#include "effectrainbow.h"
#include "effectrainbow2.h"
#include "effectfire.h"
#include "effectsnowflake.h"
#include "effectsnowglobe.h"
#include "effectheart.h"
#include "effectstart.h"
#include "effectChangingColor.h"
#include "boot.h"
#include "hardware.h"

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
/// Initializes the task system
void InitTaskSystem(void);

//-----------------------------------------------------------------------------
// Global members of the module
//-----------------------------------------------------------------------------
/// LED-effect manager
CLedEffectMgr        LedEffectMgr;
/// UDP server
CUdpServer           UdpServer;
/// Sound sensor manager
CSoundSensor         SoundSensor;
/// OTA-update manager
CRoseLightOTA        RoseLightOTA;
/// Bluetooth server
CBluetoothServer     BluetoothServer;
/// Boot task
CBoot                Boot;

// LED-effect instance
/// LED-effect 'Multi color'
CEffectMulticolor    EffectMulticolor;
/// LED-effect 'Single color'
CEffectSinglecolor   EffectSinglecolor;
/// LED-effect 'Rainbow'
CEffectRainbow       EffectRainbow;
/// LED-effect 'Rainbow 2'
CEffectRainbow2      EffectRainbow2;
/// LED-effect 'Fire'
CEffectFire          EffectFire;
/// LED-effect 'Snow flake'
CEffectSnowFlake     EffectSnowFlake;
/// LED-effect 'Snow globe'
CEffectSnowGlobe     EffectSnowGlobe;
/// LED-effect 'Heart'
CEffectHeart         EffectHeart;
/// LED-effect 'Start'
CEffectStart         EffectStart;
/// LED-effect 'Changing color'
CEffectChangingColor EffectChangingColor;
/// LED-effect 'Hardware'
CHardware            Hardware;

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Function setup
///
/// @brief Setup function of the arduino application
///
//-----------------------------------------------------------------------------
void setup()
{
   // put your setup code here, to run once:
   Serial.begin(115200);
   Serial.println("Start the system ...");

   srand(time(NULL));

   AppPreferences::begin();

   InitTaskSystem();
}

//-----------------------------------------------------------------------------
//  Function loop
///
/// @brief Loop function of the arduino application
///
//-----------------------------------------------------------------------------
void loop()
{
   // nothing to do, we have tasks in FreeRTOS
}

//-----------------------------------------------------------------------------
//  Function InitTaskSystem
///
/// @brief Initializes the task system
///
//-----------------------------------------------------------------------------
void InitTaskSystem(void)
{
   Boot.Elaborate();
}
