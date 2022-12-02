//-----------------------------------------------------------------------------
//  effectstaticcolor.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Single color'
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

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "ledeffectdefs.h"
#include "effectsinglecolor.h"
#include "ledeffectmgr.h"
#include "AppPreferences.h"
#include "soundsensor.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Effect name for 'Single color'
#define EFFECT_STATICCOLOR_NAME              "SingleColor"
/// Parameter id/index for base frequency master for effect 'Single color'
#define EFFECT_SINGLECOLOR_ISMASTERFLAG      0
/// Parameter id/index for base frequency master for effect 'Single color'
#define EFFECT_SINGLECOLOR_FREQUENCY_MASTER  1
/// Parameter id/index for base frequency slave for effect 'Single color'
#define EFFECT_SINGLECOLOR_FREQUENCY_SLAVE   2
/// Parameter id/index for flag soft start or not for effect 'Single color'
#define EFFECT_SINGLECOLOR_ISTARTTIME        3
/// Parameter id/index for brightness device type 1 for effect 'Single color'
#define EFFECT_SINGLECOLOR_BRIGHTNESS_TYPE1  4
/// Parameter id/index for color value for effect 'Single color'
#define EFFECT_SINGLECOLOR_ICOLOR            5
/// Parameter id/index for brightness device type 2 for effect 'Single color'
#define EFFECT_SINGLECOLOR_BRIGHTNESS_TYPE2  6
/// Parameter id/index for step size update LED-brightness at start phase for effect 'Single color'
#define EFFECT_SINGLECOLOR_START_STEPSIZE_MS 50

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// Instance of the CLedEffectMgr class
extern CLedEffectMgr LedEffectMgr;
/// Instance of the sound sensor
extern CSoundSensor SoundSensor;

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
//  Method CEffectSinglecolor
///
/// @brief Constructor of the class CEffectSinglecolor
///
//-----------------------------------------------------------------------------
CEffectSinglecolor::CEffectSinglecolor()
   : CLedEffectBase((uint16_t)LedEffectId::SingleColor)
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
const char * CEffectSinglecolor::GetName(void)
{
   return EFFECT_STATICCOLOR_NAME;
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
void CEffectSinglecolor::Start(void)
{
   StartEffect_SingleColor();
}

//-----------------------------------------------------------------------------
//  Method StartEffect_SingleColor
///
/// @brief Starts the LED-effect
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectSinglecolor::StartEffect_SingleColor(void)
{
   uint32_t currentTime = 0;
   double currentBrightness = 0.0f;
   double brigthnessChangePerStep;
   uint8_t levelAtFrequency;
   uint8_t frequencyValues[7];
   TickType_t xLastWakeTime;
   const TickType_t xFrequency = 50;
   LedEffectFequency effectFrequency;
   uint32_t effectBrightness;

   SetEffectParameter();

   if (2 == AppPreferences::GetType())
   {
      if ((LedEffectFequency::Off != m_FrequencyMaster
          && LedEffectFequency::Unknown != m_FrequencyMaster)
          || 1 == m_IsMaster)
      {
         SoundSensor.ActivateSoundSensor();
      }

      effectFrequency = m_FrequencyMaster;
      effectBrightness = m_BrighnessType2Pct;
   }
   else
   {
      effectFrequency = m_FrequencySlave;
      effectBrightness = m_BrighnessType1Pct;
   }

   CRGB color;

   color.red = (uint8_t)((m_Color >> 16) & 0xFF);
   color.green = (uint8_t)((m_Color >> 8) & 0xFF);
   color.blue = (uint8_t)(m_Color & 0xFF);

   brigthnessChangePerStep = (double)effectBrightness / ((double)m_StartTimeMs / EFFECT_SINGLECOLOR_START_STEPSIZE_MS);

   Serial.printf("Brightness step size 0x%f ms\r\n",brigthnessChangePerStep);

   m_IsRunning = true;

   Serial.printf("Start effect start phase\r\n");

   if (0 != m_StartTimeMs)
   {
      while((currentTime < m_StartTimeMs) && m_IsRunning)
      {
         currentTime += EFFECT_SINGLECOLOR_START_STEPSIZE_MS;

         currentBrightness += brigthnessChangePerStep;

         SetColor(color, (uint8_t)currentBrightness);

         vTaskDelay(EFFECT_SINGLECOLOR_START_STEPSIZE_MS);
      }
   }

   Serial.printf("Start effect normal\r\n");

   SetColor(color, (uint8_t)effectBrightness);

   // Initialise the xLastWakeTime variable with the current time.
   xLastWakeTime = xTaskGetTickCount();

   while(m_IsRunning)
   {
      vTaskDelayUntil( &xLastWakeTime, xFrequency );

      if(LedEffectFequency::Off == effectFrequency || LedEffectFequency::Unknown == effectFrequency)
      {
         // nothing to do
      }
      else if (LedEffectFequency::AllFrequencies == effectFrequency)
      {

      }
      else 
      {
         LedEffectMgr.GetFrequencyValues(frequencyValues);

         switch (m_FrequencyMaster)
         {
            case LedEffectFequency::Freq63Hz:
            default:   levelAtFrequency = frequencyValues[0];
            case LedEffectFequency::Freq160Hz:   levelAtFrequency = frequencyValues[1];
            case LedEffectFequency::Freq400Hz:   levelAtFrequency = frequencyValues[2];
            case LedEffectFequency::Freq1000Hz:   levelAtFrequency = frequencyValues[3];
            case LedEffectFequency::Freq2500Hz:   levelAtFrequency = frequencyValues[4];
            case LedEffectFequency::Freq6250Hz:   levelAtFrequency = frequencyValues[5];
            case LedEffectFequency::Freq16000Hz:   levelAtFrequency = frequencyValues[6];
         }

         currentBrightness = ((double)levelAtFrequency/100) * effectBrightness;

         // Maximum brightness should never be reached
         if (currentBrightness > effectBrightness)
            currentBrightness = effectBrightness;

         // Led should never be switched off
         if (currentBrightness < 1)
            currentBrightness = 1;

         SetColor(color, (uint8_t)currentBrightness);
      }
   }

   ProcessStopSequence();
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectSinglecolor::SetEffectParameter(void)
{
   m_IsMaster = LedEffectMgr.GetEffectParameter(EFFECT_SINGLECOLOR_ISMASTERFLAG);
   m_FrequencyMaster = (LedEffectFequency) LedEffectMgr.GetEffectParameter(EFFECT_SINGLECOLOR_FREQUENCY_MASTER);
   m_FrequencySlave = (LedEffectFequency) LedEffectMgr.GetEffectParameter(EFFECT_SINGLECOLOR_FREQUENCY_SLAVE);
   m_StartTimeMs = LedEffectMgr.GetEffectParameter(EFFECT_SINGLECOLOR_ISTARTTIME);
   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_SINGLECOLOR_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_SINGLECOLOR_BRIGHTNESS_TYPE2);
   m_Color = LedEffectMgr.GetEffectParameter(EFFECT_SINGLECOLOR_ICOLOR);

   Serial.printf("Starttime         0x%08x ms\r\n",m_StartTimeMs);
   Serial.printf("Brightness type 1 0x%08x %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2 0x%08x %%\r\n",m_BrighnessType2Pct);
   Serial.printf("Color             0x%08x\r\n",m_Color);
}