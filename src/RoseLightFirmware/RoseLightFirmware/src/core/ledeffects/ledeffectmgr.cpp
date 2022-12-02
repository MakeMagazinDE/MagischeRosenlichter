//-----------------------------------------------------------------------------
//  ledeffectmgr.cpp
/// @file
///
/// @brief Contains the implementation of the LED-effect manager class
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
#include "ledeffectmgr.h"

#include "effectmulticolor.h"
#include "soundsensor.h"
#include "AppPreferences.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// Number of LEDs for device type 1
#define NUM_OF_LEDS_TYPE_1    57
/// Number of LEDs for device type 2
#define NUM_OF_LEDS_TYPE_2    241
/// Maximum number of LEDs
#define MAX_NUM_OF_LEDS       NUM_OF_LEDS_TYPE_2
/// Pin for LED output
#define LED_PIN               27
/// Array to control the LED-hardware
CRGB leds[MAX_NUM_OF_LEDS];

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
/// For access to the sound sensor control instance
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
//  Method CLedEffectMgr
///
/// @brief Constructor of the class CLedEffectMgr
///
//-----------------------------------------------------------------------------
CLedEffectMgr::CLedEffectMgr()
   : m_JobLedEffectId(LedEffectId::Unknown)
   , m_JobType(LedEffectJobType::Unknown)
   , m_StopEffectSequence(StopEffectSequence::Unknown)
{
   m_LedEffectList.clear();

   if (1 == AppPreferences::GetType())
   {
      m_NumberOfLeds = NUM_OF_LEDS_TYPE_1;
   }
   else
   {
      m_NumberOfLeds = NUM_OF_LEDS_TYPE_2;
   }
}

//-----------------------------------------------------------------------------
//  Method RegisterLedEffect
///
/// @brief Registration method for led effects at the system
///
/// @param[in] pLedEffect - Instance of the effect class to register
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::RegisterLedEffect(CLedEffectBase * pLedEffect)
{
   pLedEffect->SetLedHardware(leds, m_NumberOfLeds);

   m_LedEffectList.push_back(pLedEffect);
}

//-----------------------------------------------------------------------------
//  Method StartEffect
///
/// @brief Starts a LED-effect
///
/// @param[in] effectId - Id of the effect to start
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::StartEffect(LedEffectId effectId)
{
   m_JobLedEffectId = effectId;

   m_JobType = LedEffectJobType::StartJob;

   Serial.printf("Start led effect 0x%04x\r\n", (unsigned int)effectId);

   xSemaphoreGive(m_JobSema);
}

//-----------------------------------------------------------------------------
//  Method SetSystemStartedEffect
///
/// @brief Sets the system start effect
///
/// @param[in] systemStartType - Start type of the system
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::SetSystemStartedEffect(SystemStartType systemStartType)
{
   uint32_t effectParams[20];

   effectParams[0] = 40;
   effectParams[1] = 40;

   if (systemStartType == SystemStartType::Normal)
   {
      effectParams[2] = CRGB::Blue;
      effectParams[3] = 0x00000000;
   }
   else if (systemStartType == SystemStartType::Error)
   {
      effectParams[2] = CRGB::Red;
      effectParams[3] = 0x00000001;
   }
   else
   {
      effectParams[2] = CRGB::Yellow;
      effectParams[3] = 0x00000001;
   }

   StopEffect(StopEffectSequence::HardSwitchOff);

   SetEffectParameter(effectParams);

   StartEffect(LedEffectId::Start);
}

//-----------------------------------------------------------------------------
//  Method SetEffectParameter
///
/// @brief Sets the effect parameter
///
/// @param[in] pEffectData - Effect parameter
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::SetEffectParameter(uint32_t * pEffectData)
{
   memcpy(m_EffectParameter, pEffectData, MAX_NUMBER_OF_EFFECT_PARAMETER * sizeof(uint32_t));
}

//-----------------------------------------------------------------------------
//  Method FindEffect
///
/// @brief Find an LED-effect in the list with registrated LED-effects
///
/// @param[in] effectId - LED-effect id
///
/// @return LED-effect instance
///
//-----------------------------------------------------------------------------
CLedEffectBase * CLedEffectMgr::FindEffect(LedEffectId effectId)
{
   for (std::list<CLedEffectBase *>::iterator it = m_LedEffectList.begin(); it != m_LedEffectList.end(); ++it)
   {
      if (effectId == (LedEffectId)(*it)->GetEffectId())
         return *it;
   }

   return nullptr;
}

//-----------------------------------------------------------------------------
//  Method GetEffectParameter
///
/// @brief Gets the effect parameter for an LED-effect
///
/// @param[in] index - Parameter index
///
/// @return Effekt parameter value
///
//-----------------------------------------------------------------------------
uint32_t CLedEffectMgr::GetEffectParameter(uint8_t index)
{
   if (index > MAX_NUMBER_OF_EFFECT_PARAMETER - 1)
      return 0xFFFFFFFF;

   return m_EffectParameter[index];
}

//-----------------------------------------------------------------------------
//  Method StopAllRunningEffects
///
/// @brief Stops all running effects
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::StopAllRunningEffects(void)
{
   for (std::list<CLedEffectBase *>::iterator it = m_LedEffectList.begin(); it != m_LedEffectList.end(); ++it)
   {
      if ((*it)->IsRunning())
      {
         (*it)->Stop(m_StopEffectSequence);
      }
   }
}

//-----------------------------------------------------------------------------
//  Method SetFrequencyValues
///
/// @brief Sets the frequency values
///
/// @param[in] pFrequencyValues - Current frequency values from the equalizer
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::SetFrequencyValues(uint8_t * pFrequencyValues)
{
   xSemaphoreTake(m_LedFrequencyValuesMutex, portMAX_DELAY);

   memcpy(m_FrequencyValues, pFrequencyValues, 7);

   xSemaphoreGive(m_LedFrequencyValuesMutex);
}

//-----------------------------------------------------------------------------
//  Method SetColorToEffect
///
/// @brief Sets the color to effect
///
/// @param[in] color - Color
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::SetColorToEffect(uint32_t color)
{
   Serial.printf("SetColor to 0x%08x\n", color);

   if (m_JobLedEffectId == LedEffectId::ChangingColor)
   {
      if (NULL != m_pRunningLedEffect)
      {
         m_pRunningLedEffect->SetColorToEffect(color);
      }
   }
}

//-----------------------------------------------------------------------------
//  Method GetFrequencyValues
///
/// @brief Gets the current frequency values
///
/// @param[out] pFrequencyValues - Frequency values
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::GetFrequencyValues(uint8_t * pFrequencyValues)
{
   xSemaphoreTake(m_LedFrequencyValuesMutex, portMAX_DELAY);

   memcpy(pFrequencyValues, m_FrequencyValues, 7);

   xSemaphoreGive(m_LedFrequencyValuesMutex);
}

//-----------------------------------------------------------------------------
//  Method StopEffect
///
/// @brief Stops the current effect
///
/// @param[in] stopEffectSequence - Stop effect sequence
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::StopEffect(StopEffectSequence stopEffectSequence)
{
   m_StopEffectSequence = stopEffectSequence;

   StopAllRunningEffects();
}

//-----------------------------------------------------------------------------
//  Method TaskFct
///
/// @brief Task function of the LED-effect manager
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::TaskFct (void)
{
   Serial.println("Start led effect manager ...");

   for(;;)
   {
      xSemaphoreTake(m_JobSema, portMAX_DELAY);

      switch (m_JobType)
      {
         case LedEffectJobType::StartJob:
            CLedEffectBase * pLedEffect;

            pLedEffect = FindEffect(m_JobLedEffectId);

            if (nullptr != pLedEffect)
            {
               Serial.printf("Start effect: %s\n\r", pLedEffect->GetName());

               m_pRunningLedEffect = pLedEffect;

               pLedEffect->Start();

               m_pRunningLedEffect = NULL;

               Serial.printf("End effect: %s\n\r", pLedEffect->GetName());
            }
            else
            {
               Serial.printf("Can't find effect with id %i!\n\r", (int)m_JobLedEffectId);
            }
            break;

         case LedEffectJobType::StopJob:
            StopAllRunningEffects();
            break;
         
         default:
            break;
      }
   }

   Serial.println("Led effect manager is stopped!");
}

//-----------------------------------------------------------------------------
//  Method Elaborate
///
/// @brief Elaborate method of the class
///
//-----------------------------------------------------------------------------
void CLedEffectMgr::Elaborate (void)
{
   Serial.println("CLedEffectMgr::Elaborate()");

   m_JobSema = xSemaphoreCreateBinary();
   m_LedFrequencyValuesMutex = xSemaphoreCreateBinary();

   FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, m_NumberOfLeds);
   FastLED.setBrightness(50); 

   SetName("tEffectMr");
   SetStackSize(16384);
   SetPriority(2);
   Startup(0);

   xSemaphoreGive(m_LedFrequencyValuesMutex);
}
