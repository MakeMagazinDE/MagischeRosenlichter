//-----------------------------------------------------------------------------
//  effectheart.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Heart'
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
#include "effectheart.h"
#include "ledeffectmgr.h"
#include "AppPreferences.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Effect name for 'Heart'
#define EFFECT_HEART_NAME              "Heart"
/// Parameter id/index for brightness device type 1 for effect 'Heart'
#define EFFECT_HEART_BRIGHTNESS_TYPE1  0
/// Parameter id/index for brightness device type 2 for effect 'Heart'
#define EFFECT_HEART_BRIGHTNESS_TYPE2  1
/// Parameter id/index for heart color for effect 'Heart'
#define EFFECT_HEART_COLOR_HEART       2
/// Parameter id/index for bacjground color for effect 'Heart'
#define EFFECT_HEART_COLOR_BACKGROUND  3

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
/// Heart mask for device type 1
uint8_t m_HeartMaskTyp1[] =
  { 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0 };

/// Heart mask for device type 2
uint8_t m_HeartMaskTyp2[] =
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
    0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
    0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1,
    0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1,
    0, 1, 0, 1, 0, 1, 0, 1,
    0 };

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Method CEffectHeart
///
/// @brief Constructor of the class CEffectHeart
///
//-----------------------------------------------------------------------------
CEffectHeart::CEffectHeart()
   : CLedEffectBase((uint16_t)LedEffectId::Heart)
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
void CEffectHeart::Start(void)
{
   SetEffectParameter();

   StartEffect_Heart();
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the name of the effect
///
/// @return Name of the effect
///
//-----------------------------------------------------------------------------
const char * CEffectHeart::GetName(void)
{
   return EFFECT_HEART_NAME;
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectHeart::SetEffectParameter(void)
{
   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_HEART_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_HEART_BRIGHTNESS_TYPE2);
   m_ColorHeart = LedEffectMgr.GetEffectParameter(EFFECT_HEART_COLOR_HEART);
   m_ColorBackground = LedEffectMgr.GetEffectParameter(EFFECT_HEART_COLOR_BACKGROUND);

   Serial.printf("Brightness type 1 %i %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2 %i %%\r\n",m_BrighnessType2Pct);
   Serial.printf("Color heart       0x%08x\r\n",m_ColorHeart);
   Serial.printf("Color background  0x%08x\r\n",m_ColorBackground);
}

//-----------------------------------------------------------------------------
//  Method StartEffect_Heart
///
/// @brief Starts the LED-effect
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectHeart::StartEffect_Heart(void)
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

      if (2 == AppPreferences::GetType())
      {
         for (int i =0; i < 241; i++)
         {
            if (m_HeartMaskTyp2[i] == 1)
            {
               colors[i] = m_ColorHeart;
            }
            else
            {
               colors[i] = m_ColorBackground;
            }
         }

         AddColor2FromTable(&colors[0], 241, (uint16_t)0, 0, 241, 1.0f);
         ShowColors((uint8_t)effectBrightness);
      }
      else
      {
         for (int i =0; i < 57; i++)
         {
            if (m_HeartMaskTyp1[i] == 1)
            {
               colors[i] = m_ColorHeart;
            }
            else
            {
               colors[i] = m_ColorBackground;
            }
         }

         AddColor2FromTable(&colors[0], 57, (uint16_t)0, 0, 57, 1.0f);
         ShowColors((uint8_t)effectBrightness);
      }
   }

   ProcessStopSequence();
}