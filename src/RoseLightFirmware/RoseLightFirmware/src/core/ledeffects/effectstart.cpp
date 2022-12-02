//-----------------------------------------------------------------------------
//  effectstart.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Start'
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
#include "effectstart.h"
#include "ledeffectmgr.h"
#include "AppPreferences.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Effect name for 'Start'
#define EFFECT_START_NAME              "Start"
/// Parameter id/index for brightness device type 1 for effect 'Start'
#define EFFECT_START_BRIGHTNESS_TYPE1  0
/// Parameter id/index for brightness device type 2 for effect 'Start'
#define EFFECT_START_BRIGHTNESS_TYPE2  1
/// Parameter id/index for color for effect 'Start'
#define EFFECT_START_COLOR_HEART       2
/// Parameter id/index for flag 'Run forever' for effect 'Start'
#define EFFECT_START_RUN_FOREVER       3
/// Parameter id/index for start time for effect 'Start'
#define EFFECT_START_TIME              40

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
//  Method CEffectStart
///
/// @brief Constructor for the class CEffectStart
///
//-----------------------------------------------------------------------------
CEffectStart::CEffectStart()
   : CLedEffectBase((uint16_t)LedEffectId::Start)
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
void CEffectStart::Start(void)
{
   SetEffectParameter();

   StartEffect_Start();
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the name of the effect
///
/// @return Name of the effect
///
//-----------------------------------------------------------------------------
const char * CEffectStart::GetName(void)
{
   return EFFECT_START_NAME;
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectStart::SetEffectParameter(void)
{
   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_START_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_START_BRIGHTNESS_TYPE2);
   m_Color = LedEffectMgr.GetEffectParameter(EFFECT_START_COLOR_HEART);
   m_RunForever = LedEffectMgr.GetEffectParameter(EFFECT_START_RUN_FOREVER);

   Serial.printf("Brightness type 1 %i %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2 %i %%\r\n",m_BrighnessType2Pct);
   Serial.printf("Color             0x%08x\r\n",m_Color);
   Serial.printf("Run forever       0x%08x\r\n",m_RunForever);
}
   
//-----------------------------------------------------------------------------
//  Method StartEffect_Start
///
/// @brief Starts the LED-effect
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectStart::StartEffect_Start(void)
{
   TickType_t xLastWakeTime;
   const TickType_t xFrequency = 50;
   uint32_t effectBrightness;
   float currentBrightness;
   float brightnessStepSize;
   uint16_t i;

   if (2 == AppPreferences::GetType())
   {
      effectBrightness = m_BrighnessType2Pct;
   }
   else
   {
      effectBrightness = m_BrighnessType1Pct;
   }

   m_IsRunning = true;

   Serial.printf("Start effect\r\n");

   // Initialise the xLastWakeTime variable with the current time.
   xLastWakeTime = xTaskGetTickCount();

   currentBrightness = 0;

   brightnessStepSize = (float)effectBrightness / EFFECT_START_TIME;

   for (i = 0; i < EFFECT_START_TIME; i++)
   {
      vTaskDelayUntil( &xLastWakeTime, xFrequency );

      currentBrightness += brightnessStepSize;

      SetColor(m_Color, (uint8_t)currentBrightness);
   }

   if (m_RunForever != 0x00000001)
   {
      currentBrightness = effectBrightness;

      for (i = 0; i < EFFECT_START_TIME; i++)
      {
         vTaskDelayUntil( &xLastWakeTime, xFrequency );

         currentBrightness -= brightnessStepSize;

         SetColor(m_Color, (uint8_t)currentBrightness);
      }
   }
   else
   {
      while(1)
      {
         vTaskDelayUntil( &xLastWakeTime, xFrequency );
      }
   }
}
