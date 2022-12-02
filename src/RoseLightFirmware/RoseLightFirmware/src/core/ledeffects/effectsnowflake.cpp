//-----------------------------------------------------------------------------
//  effectsnowflake.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Snow flake'
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
#include "effectsnowflake.h"
#include "ledeffectmgr.h"
#include "AppPreferences.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Effect name for 'Snow flake'
#define EFFECT_SNOWFLAKE_NAME                "Snowflake"
/// Parameter id/index for brightness device type 1 for effect 'Snow flake'
#define EFFECT_SNOWFLAKE_BRIGHTNESS_TYPE1    0
/// Parameter id/index for brightness device type 2 for effect 'Snow flake'
#define EFFECT_SNOWFLAKE_BRIGHTNESS_TYPE2    1
/// Parameter id/index for color of the snow flake for effect 'Snow flake'
#define EFFECT_SNOWFLAKE_COLOR_SNOWFLAKE     2
/// Parameter id/index for background color for effect 'Snow flake'
#define EFFECT_SNOWFLAKE_COLOR_BACKGROUND    3
/// Parameter id/index for color of a star for effect 'Snow flake'
#define EFFECT_SNOWFLAKE_COLOR_STAR          4
/// Parameter id/index for step size for the effect start phase for effect 'Snow flake'
#define EFFECT_SNOWFLAKE_START_STEPSIZE_MS   50

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// Instance of the CLedEffectMgr class
extern CLedEffectMgr LedEffectMgr;

//-----------------------------------------------------------------------------
// Prototypes of private functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global members of the module
//-----------------------------------------------------------------------------
/// Snow flake mask
uint8_t m_SnowFlakeMaskTyp2[] =
  { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
    1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1,
    1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0, 1, 0, 2, 0,
    1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2, 1, 2, 2,
    2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 1, 0, 1, 0, 1, 0,
    0 };

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Method CEffectSnowFlake
///
/// @brief Constructor of the class CEffectSnowFlake
///
//-----------------------------------------------------------------------------
CEffectSnowFlake::CEffectSnowFlake()
   : CLedEffectBase((uint16_t)LedEffectId::SnowFlake)
{
}

//-----------------------------------------------------------------------------
//  Method Start
///
/// @brief Sets the effect parameter and starts the effect
///
/// This method sets the effect parameter and starts the effect.
///
/// @see SetEffectParameter()
///
//-----------------------------------------------------------------------------
void CEffectSnowFlake::Start(void)
{
   SetEffectParameter();

   StartEffect_SnowFlake();
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the name of the effect
///
/// @return Name of the effect
///
//-----------------------------------------------------------------------------
const char * CEffectSnowFlake::GetName(void)
{
   return EFFECT_SNOWFLAKE_NAME;
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectSnowFlake::SetEffectParameter(void)
{
   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_SNOWFLAKE_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_SNOWFLAKE_BRIGHTNESS_TYPE2);
   m_ColorSnowFlake = LedEffectMgr.GetEffectParameter(EFFECT_SNOWFLAKE_COLOR_SNOWFLAKE);
   m_ColorBackground = LedEffectMgr.GetEffectParameter(EFFECT_SNOWFLAKE_COLOR_BACKGROUND);
   m_ColorStar = LedEffectMgr.GetEffectParameter(EFFECT_SNOWFLAKE_COLOR_STAR);

   Serial.printf("Brightness type 1 %i %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2 %i %%\r\n",m_BrighnessType2Pct);
   Serial.printf("Color Snowflake   0x%08x\r\n",m_ColorSnowFlake);
   Serial.printf("Color Background  0x%08x\r\n",m_ColorBackground);
   Serial.printf("Color Star        0x%08x\r\n",m_ColorStar);
}
   
//-----------------------------------------------------------------------------
//  Method StartEffect_SnowFlake
///
/// @brief Starts the LED-effect
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectSnowFlake::StartEffect_SnowFlake(void)
{
   TickType_t xLastWakeTime;
   const TickType_t xFrequency = 50;
   uint32_t effectBrightness;
   uint32_t colors[241];

   if (2 == AppPreferences::GetType())
   {
      effectBrightness = m_BrighnessType2Pct;
   }
   else
   {
      effectBrightness = m_BrighnessType1Pct;
   }

   m_IsRunning = true;

   Serial.printf("Start effect normal\r\n");

   // Initialise the xLastWakeTime variable with the current time.
   xLastWakeTime = xTaskGetTickCount();

   while(m_IsRunning)
   {
      vTaskDelayUntil( &xLastWakeTime, xFrequency );

      for (int i =0; i < 241; i++)
      {
         if (m_SnowFlakeMaskTyp2[i] == 1)
         {
            colors[i] = m_ColorSnowFlake;
         }
         else if (m_SnowFlakeMaskTyp2[i] == 2)
         {
            colors[i] = m_ColorStar;
         }
         else
         {
            colors[i] = m_ColorBackground;
         }
      }

      AddColor2FromTable(&colors[0], 241, (uint16_t)0, 0, 241, 1.0f);
      ShowColors((uint8_t)effectBrightness);
   }

   ProcessStopSequence();
}