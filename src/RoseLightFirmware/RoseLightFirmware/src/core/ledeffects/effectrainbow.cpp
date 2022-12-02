//-----------------------------------------------------------------------------
//  effectrainbow.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Rainbow'
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
#include "effectrainbow.h"
#include "AppPreferences.h"
#include "ledeffectmgr.h"
#include "soundsensor.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Effect name for 'Rainbow'
#define EFFECT_RAINBOW_NAME               "Rainbow"
/// Parameter id/index for rotation time for effect 'Rainbow'
#define EFFECT_RAINBOW_TIME_ROTATION      5

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
//  Method CEffectRainbow
///
/// @brief Constructor of the class CEffectRainbow
///
//-----------------------------------------------------------------------------
CEffectRainbow::CEffectRainbow()
   : CEffectRainbowBase((uint16_t)LedEffectId::Rainbow)
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
void CEffectRainbow::Start(void)
{
   StartEffect_Rainbow();
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the name of the effect
///
/// @return Name of the effect
///
//-----------------------------------------------------------------------------
const char * CEffectRainbow::GetName(void)
{
   return EFFECT_RAINBOW_NAME;
}

//-----------------------------------------------------------------------------
//  Method StartEffect_Rainbow
///
/// @brief Starts the LED-effect
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectRainbow::StartEffect_Rainbow(void)
{ 
   uint32_t effectBrightness;
   TickType_t xLastWakeTime;
   const TickType_t xFrequency = 50;
   LedEffectFequency effectFrequency;
   uint8_t frequencyValues[7];
   uint8_t levelAtFrequency;
   double currentBrightness = 0.0f;

   SetEffectParameter();

   m_IsRunning = true;

   if (2 == AppPreferences::GetType())
   {
      if ((LedEffectFequency::Off != m_FrequencyMaster
          && LedEffectFequency::Unknown != m_FrequencyMaster)
          || 1 == m_IsMaster)
      {
         SoundSensor.ActivateSoundSensor();
      }

      effectBrightness = m_BrighnessType2Pct;
      effectFrequency = m_FrequencyMaster;
   }
   else
   {
      effectBrightness = m_BrighnessType1Pct;
      effectFrequency = m_FrequencySlave;
   }

   float stepSizeRotation = ((float)360) / ((m_TimeRotationS) * (1000 / 50));

   Serial.printf("Step size rotation: %f\n\r", stepSizeRotation);

   float startInd = 0.0f;

   // Initialise the xLastWakeTime variable with the current time.
   xLastWakeTime = xTaskGetTickCount();

   while(m_IsRunning)
   {
      vTaskDelayUntil( &xLastWakeTime, xFrequency );

      LedEffectMgr.GetFrequencyValues(frequencyValues);

      if (0 == m_IsMaster || LedEffectFequency::Off == m_FrequencyMaster) 
      {
         currentBrightness = effectBrightness;
      }
      else
      {
         switch (effectFrequency)
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
         if (currentBrightness < 3)
            currentBrightness = 3;
      }

      if (2 == AppPreferences::GetType())
      {
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 0, 60, ((float)360)/(60));
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 60, 48, ((float)360)/(48));
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 108, 40, ((float)360)/(40));
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 148, 32, ((float)360)/(32));
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 180, 24, ((float)360)/(24));
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 204, 16, ((float)360)/(16));
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 220, 12, ((float)360)/(12));
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 232, 8, ((float)360)/(8));
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 240, 1, ((float)360)/(1));
         ShowColors((uint8_t)currentBrightness);
      }
      else
      {
         AddColor2FromTable(RingColors, 360, (uint16_t)startInd, 0, 57, ((float)360)/(57));
         ShowColors((uint8_t)currentBrightness);
      }

      startInd += stepSizeRotation;

      if (startInd >= 360)
      {
         startInd -= 360;
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
void CEffectRainbow::SetEffectParameter(void)
{
   CEffectRainbowBase::SetEffectParameter();

   m_TimeRotationS = LedEffectMgr.GetEffectParameter(EFFECT_RAINBOW_TIME_ROTATION);

   Serial.printf("Time rotation     %i s\r\n",m_TimeRotationS);
}