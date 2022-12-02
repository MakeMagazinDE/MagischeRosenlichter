//-----------------------------------------------------------------------------
//  effectChangingColor.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect 'Changing color'
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
#include "effectChangingColor.h"
#include "ledeffectmgr.h"
#include "AppPreferences.h"
#include "udpserver.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Effect name for 'Changing color'
#define EFFECT_CHANGINGCOLOR_NAME                  "ChangingColor"
/// Parameter id/index for brightness device type 1 for effect 'Changing color'
#define EFFECT_CHANGINGCOLOR_BRIGHTNESS_TYPE1      0
/// Parameter id/index for brightness device type 2 for effect 'Changing color'
#define EFFECT_CHANGINGCOLOR_BRIGHTNESS_TYPE2      1
/// Parameter id/index for color 1 for effect 'Changing color'
#define EFFECT_CHANGINGCOLOR_COLOR1                2
/// Parameter id/index for color 2 for effect 'Changing color'
#define EFFECT_CHANGINGCOLOR_COLOR2                3
/// Parameter id/index for color view time for effect 'Changing color'
#define EFFECT_CHANGINGCOLOR_COLORVIEWTIME         4
/// Parameter id/index for flag 'Is master controlled' for effect 'Changing color'
#define EFFECT_CHANGINGCOLOR_ISMASTERCONTROLLED    5

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// For access to the LED-effect manager instance
extern CLedEffectMgr LedEffectMgr;
/// For access to the UDP-server instance
extern CUdpServer UdpServer;

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
//  Method CEffectChangingColor
///
/// @brief Constructor of the class CEffectChangingColor
///
//-----------------------------------------------------------------------------
CEffectChangingColor::CEffectChangingColor()
   : CLedEffectBase((uint16_t)LedEffectId::ChangingColor)
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
void CEffectChangingColor::Start(void)
{
   SetEffectParameter();

   StartEffect_ChangingColor();
}

//-----------------------------------------------------------------------------
//  Method GetName
///
/// @brief Gets the name of the effect
///
/// @return Name of the effect
///
//-----------------------------------------------------------------------------
const char * CEffectChangingColor::GetName(void)
{
   return EFFECT_CHANGINGCOLOR_NAME;
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectChangingColor::SetEffectParameter(void)
{
   m_BrighnessType1Pct = LedEffectMgr.GetEffectParameter(EFFECT_CHANGINGCOLOR_BRIGHTNESS_TYPE1);
   m_BrighnessType2Pct = LedEffectMgr.GetEffectParameter(EFFECT_CHANGINGCOLOR_BRIGHTNESS_TYPE2);

   m_Color1 = LedEffectMgr.GetEffectParameter(EFFECT_CHANGINGCOLOR_COLOR1);
   m_Color2 = LedEffectMgr.GetEffectParameter(EFFECT_CHANGINGCOLOR_COLOR2);

   m_ColorViewTime = LedEffectMgr.GetEffectParameter(EFFECT_CHANGINGCOLOR_COLORVIEWTIME);
   m_IsMasterControlled = LedEffectMgr.GetEffectParameter(EFFECT_CHANGINGCOLOR_ISMASTERCONTROLLED);
   
   Serial.printf("Brightness type 1    %i %%\r\n",m_BrighnessType1Pct);
   Serial.printf("Brightness type 2    %i %%\r\n",m_BrighnessType2Pct);
   Serial.printf("Color 1              0x%08x\r\n",m_Color1);
   Serial.printf("Color 2              0x%08x\r\n",m_Color2);
   Serial.printf("Color view time      %i ms\r\n",m_ColorViewTime);
   Serial.printf("Master controlled    %i ms\r\n",m_IsMasterControlled);
}
   
//-----------------------------------------------------------------------------
//  Method StartEffect_ChangingColor
///
/// @brief Starts the LED-effect
///
/// @see Start()
///
//-----------------------------------------------------------------------------
void CEffectChangingColor::StartEffect_ChangingColor(void)
{
   CRGB color;
   uint32_t effectBrightness;
   TickType_t xLastWakeTime;

   m_IsRunning = true;

   if (2 == AppPreferences::GetType())
   {
      effectBrightness = m_BrighnessType2Pct;
   }
   else
   {
      effectBrightness = m_BrighnessType1Pct;
   }

   xLastWakeTime = xTaskGetTickCount();

   while(m_IsRunning)
   {
      if (2 == AppPreferences::GetType() && 1 == m_IsMasterControlled)
      {
         UdpServer.SendSetColorMsg(m_Color1);

         SetColor(m_Color1, (uint8_t)effectBrightness);

         vTaskDelayUntil( &xLastWakeTime, m_ColorViewTime );

         UdpServer.SendSetColorMsg(m_Color2);

         SetColor(m_Color2, (uint8_t)effectBrightness);

         vTaskDelayUntil( &xLastWakeTime, m_ColorViewTime );
      }
      else if (1 == AppPreferences::GetType() && 1 == m_IsMasterControlled)
      {
         vTaskDelayUntil( &xLastWakeTime, 50 );
      }
      else
      {
         SetColor(m_Color1, (uint8_t)effectBrightness);

         vTaskDelayUntil( &xLastWakeTime, m_ColorViewTime );

         SetColor(m_Color2, (uint8_t)effectBrightness);

         vTaskDelayUntil( &xLastWakeTime, m_ColorViewTime );
      }
   }

   ProcessStopSequence();
}

//-----------------------------------------------------------------------------
//  Method SetColorToEffect
///
/// @brief Sets the color to the effect
///
/// @param[in] color - New LED-color value
///
//-----------------------------------------------------------------------------
void CEffectChangingColor::SetColorToEffect(uint32_t color)
{
   SetColor(color, (uint8_t)m_BrighnessType1Pct);
}