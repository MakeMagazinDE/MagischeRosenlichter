//-----------------------------------------------------------------------------
//  effectfire.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Fire'
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "effectfire.h"
#include "AppPreferences.h"
#include "ledeffectmgr.h"

/// FastLED definitions
#define NUM_LEDS           240

#include "fire.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Effect name for 'Fire'
#define EFFECT_FIRE_NAME               "Fire"
/// Parameter id/index for brightness device type 1 for effect 'Fire'
#define EFFECT_FIRE_BRIGHTNESS_TYPE1   0
/// Parameter id/index for brightness device type 2 for effect 'Fire'
#define EFFECT_FIRE_BRIGHTNESS_TYPE2   1
/// Parameter id/index for time redraw rate for effect 'Fire'
#define EFFECT_FIRE_START_STEPSIZE_MS  50

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
//  Method CEffectFire
///
/// @brief Constructor of the class CEffectFire
//-----------------------------------------------------------------------------
CEffectFire::CEffectFire()
   : CLedEffectBase((uint16_t)LedEffectId::Fire)
{
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the the name of the effect
///
/// @return Name of the effect
///
//-----------------------------------------------------------------------------
const char * CEffectFire::GetName(void)
{
   return EFFECT_FIRE_NAME;
}

//-----------------------------------------------------------------------------
//  Method Start
///
/// @brief Starts the effect
///
/// @see StartEffect_Fire()
///
//-----------------------------------------------------------------------------
void CEffectFire::Start(void)
{
   StartEffect_Fire();
}

//-----------------------------------------------------------------------------
//  Method StartEffect_Fire
///
/// @brief Starts the effect 'fire'
///
/// @see SetEffectParameter()
///
//-----------------------------------------------------------------------------
void CEffectFire::StartEffect_Fire(void)
{
   TickType_t xLastWakeTime;
   uint32_t effectBrightness;

   const TickType_t xFrequency = EFFECT_FIRE_START_STEPSIZE_MS;

   SetEffectParameter();

   if (2 == AppPreferences::GetType())
   {
      effectBrightness = m_BrighnessType2Pct;
   }
   else
   {
      effectBrightness = m_BrighnessType1Pct;
   }

   m_IsRunning = true;
   //ClassicFireEffect fire(NUM_LEDS, 30, 100, 3, 2, false, true);         // Outwards from Middle
   //ClassicFireEffect fire(NUM_LEDS, 50, 300, 30, 12, true, false);       // More Intense, Extra Sparking
   //ClassicFireEffect fire(NUM_LEDS, 20, 150, 1, 4, true, false);         // Outwards from Zero
   ClassicFireEffect fire(NUM_LEDS, 40, 100, 3, NUM_LEDS, true, false);    // Fan with correct rotation

   //ClassicFireEffect fire(NUM_LEDS);

   // Initialise the xLastWakeTime variable with the current time.
   xLastWakeTime = xTaskGetTickCount();

   while(m_IsRunning)
   {
      vTaskDelayUntil( &xLastWakeTime, xFrequency );

      FastLED.clear();
      fire.DrawFire();
      FastLED.show(effectBrightness);                          //  Show and delay
   }

   ProcessStopSequence();
}

//-----------------------------------------------------------------------------
//  Method DrawMarqueeComparison
///
/// @brief <Please fill out with the short description!>
///
/// <Please fill out with the extended description!>
///
/// @see <References, optional>
///
/// @todo <This comment was placed in by a macro. Please fill out all the field
///        in brackets with useful information.>
//-----------------------------------------------------------------------------
void CEffectFire::DrawMarqueeComparison()
{
  static float scroll = 0.0f;
  scroll += 0.1f;
  if (scroll > 5.0)
    scroll -= 5.0;

  for (float i = scroll; i < NUM_LEDS/2 -1; i+= 5)
  {
    DrawPixels(i, 3, CRGB::Green);
    DrawPixels(NUM_LEDS-1-(int)i, 3, CRGB::Red);
  }
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
//-----------------------------------------------------------------------------
void CEffectFire::SetEffectParameter(void)
{
   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_FIRE_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_FIRE_BRIGHTNESS_TYPE2);

   Serial.printf("Brightness type 1 0x%08x %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2 0x%08x %%\r\n",m_BrighnessType2Pct);
}