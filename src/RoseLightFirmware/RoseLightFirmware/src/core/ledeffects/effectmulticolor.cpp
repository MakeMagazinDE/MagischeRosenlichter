//-----------------------------------------------------------------------------
//  effectmulticolor.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Multi color'
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <Arduino.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "ledeffectdefs.h"
#include "effectmulticolor.h"
#include "ledeffectmgr.h"
#include "AppPreferences.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Name of the effect
#define EFFECT_MULTICOLOR_NAME            "MultiColor"
// Index parameters in parameter set
/// Index parameter color 1
#define EFFECT_MULTICOLOR_COLOR1          0
/// Index parameter color 2
#define EFFECT_MULTICOLOR_COLOR2          1
/// Index parameter color 3
#define EFFECT_MULTICOLOR_COLOR3          2
/// Index parameter time for color on
#define EFFECT_MULTICOLOR_TIME_COLOR      3
/// Index parameter time for switch from one color to the other
#define EFFECT_MULTICOLOR_TIME_SWITCH     4
/// Index parameter brightness rose light devices type 1
#define EFFECT_MULTICOLOR_BRIGHTNESS_TYPE1 5
/// Index parameter brightness rose light devices type 2
#define EFFECT_MULTICOLOR_BRIGHTNESS_TYPE2 6

/// Time redraw rate
#define EFFECT_MULTICOLOR_STEPSIZE_MS     50

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

//-----------------------------------------------------------------------------
// Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Method CEffectMulticolor
///
/// @brief Constructor of the class CEffectMulticolor
///
//-----------------------------------------------------------------------------
CEffectMulticolor::CEffectMulticolor()
   : CLedEffectBase((uint16_t)LedEffectId::MultiColor)
{
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the name of the effect
///
/// @return Name of the effect
///
//-----------------------------------------------------------------------------
const char * CEffectMulticolor::GetName(void)
{
   return EFFECT_MULTICOLOR_NAME;
}

//-----------------------------------------------------------------------------
//  Method Start
///
/// @brief Starts the effect
///
/// @see StartEffect_MultiColor()
///
//-----------------------------------------------------------------------------
void CEffectMulticolor::Start(void)
{
   StartEffect_MultiColor();
}

//-----------------------------------------------------------------------------
//  Method StartEffect_MultiColor
///
/// @brief Starts the effect 'MultiColor'
///
/// @see SetEffectParameter()
///
//-----------------------------------------------------------------------------
void CEffectMulticolor::StartEffect_MultiColor(void)
{
   CRGB color;
   int iStep = 0; 
   uint32_t effectBrightness;
   uint32_t currentTime = 0;
   double currentBrightness = 0.0f;
   double brigthnessChangePerStep = 1.0f;

   SetEffectParameter();

   if (2 == AppPreferences::GetType())
   {
      effectBrightness = m_BrighnessType2Pct;
   }
   else
   {
      effectBrightness = m_BrighnessType1Pct;
   }

   if (0 != m_TimeSwitch)
   {
      brigthnessChangePerStep = (double)effectBrightness / ((double)m_TimeSwitch / EFFECT_MULTICOLOR_STEPSIZE_MS);
   }

   m_IsRunning = true;

   Serial.printf("Start effect normal\r\n");

   while(m_IsRunning)
   {
      switch (iStep)
      {
         case 0: color = m_Color1; break;
         case 1: color = m_Color2; break;
         case 2: color = m_Color3; break;
      }

      currentBrightness = 0.0f;
      currentTime = 0.0f;

      if (0 != m_TimeSwitch)
      {
         while((currentTime < m_TimeSwitch) && m_IsRunning)
         {
            currentTime += EFFECT_MULTICOLOR_STEPSIZE_MS;

            currentBrightness += brigthnessChangePerStep;

            SetColor(color, (uint8_t)currentBrightness);

            vTaskDelay(EFFECT_MULTICOLOR_STEPSIZE_MS);
         }
      }

      vTaskDelay(m_TimeColor);

      currentBrightness = effectBrightness;
      currentTime = 0.0f;

      if (0 != m_TimeSwitch)
      {
         while((currentTime < m_TimeSwitch) && m_IsRunning)
         {
            currentTime += EFFECT_MULTICOLOR_STEPSIZE_MS;

            currentBrightness -= brigthnessChangePerStep;

            SetColor(color, (uint8_t)currentBrightness);

            vTaskDelay(EFFECT_MULTICOLOR_STEPSIZE_MS);
         }
      }

      iStep++;

      if (iStep == 3)
      {
         iStep = 0;
      }
   }

   ProcessStopSequence();
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
//-----------------------------------------------------------------------------
void CEffectMulticolor::SetEffectParameter(void)
{
   m_Color1 = LedEffectMgr.GetEffectParameter(EFFECT_MULTICOLOR_COLOR1);
   m_Color2 = LedEffectMgr.GetEffectParameter(EFFECT_MULTICOLOR_COLOR2);
   m_Color3 = LedEffectMgr.GetEffectParameter(EFFECT_MULTICOLOR_COLOR3);

   m_TimeColor = LedEffectMgr.GetEffectParameter(EFFECT_MULTICOLOR_TIME_COLOR);
   m_TimeSwitch = LedEffectMgr.GetEffectParameter(EFFECT_MULTICOLOR_TIME_SWITCH);
   
   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_MULTICOLOR_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_MULTICOLOR_BRIGHTNESS_TYPE2);

   Serial.printf("Color 1              0x%08x\r\n",m_Color1);
   Serial.printf("Color 2              0x%08x\r\n",m_Color2);
   Serial.printf("Color 3              0x%08x\r\n",m_Color3);
   Serial.printf("Time color           %i ms\r\n",m_TimeColor);
   Serial.printf("Time swicth          %i ms\r\n",m_TimeSwitch);
   Serial.printf("Brightness type 1    %i %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2    %i %%\r\n",m_BrighnessType2Pct);
}