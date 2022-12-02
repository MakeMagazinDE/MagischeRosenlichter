//-----------------------------------------------------------------------------
//  ledeffectmgr.h
/// @file
///
/// @brief Header for the module ledeffectmgr.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __LEDEFFECTMGR_H
 #define __LEDEFFECTMGR_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <list>

#include "task.h"

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "ledeffectbase.h"

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------
/// Job type for the LED-effect manager
enum class LedEffectJobType
{
   Unknown = 0,            ///< Unknown job type
   StartJob,               ///< Job: Start LED-effect
   StopJob                 ///< Job: Stop LED-effect
};

/// Maximum number of effect parameter
#define MAX_NUMBER_OF_EFFECT_PARAMETER    20

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Class CLedEffectMgr
///
/// @brief Manager class for the LED-effect processing
///
//-----------------------------------------------------------------------------
class CLedEffectMgr : public CTask
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CLedEffectMgr();
   // Registration method for led effects at the system
   void RegisterLedEffect(CLedEffectBase * pLedEffect);
   // Starts a LED-effect
   void StartEffect(LedEffectId effectId);
   // Stops the current effect
   void StopEffect(StopEffectSequence stopEffectSequence);
   // Elaborate method of the class
   void Elaborate (void);
   // Sets the effect parameter
   void SetEffectParameter(uint32_t * pEffectData);
   // Sets the frequency values
   void SetFrequencyValues(uint8_t * pFrequencyValues);
   // Sets the color to effect
   void SetColorToEffect(uint32_t color);
   // Gets the effect parameter for an LED-effect
   uint32_t GetEffectParameter(uint8_t index);
   // Gets the current frequency values
   void GetFrequencyValues(uint8_t * pFrequencyValues);
   // Sets the system start effect6
   void SetSystemStartedEffect(SystemStartType systemStartType);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Find an LED-effect in the list with registrated KLED-effects
   CLedEffectBase * FindEffect(LedEffectId effectId);
   // Stops all running effects
   void StopAllRunningEffects(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// LED-effect list
   std::list<CLedEffectBase *> m_LedEffectList;
   /// Semaphore to initiate a new task job
   SemaphoreHandle_t m_JobSema;
   /// Mutex to save the access to the current frequency values
   SemaphoreHandle_t m_LedFrequencyValuesMutex;
   /// LED-effect id for the new job
   LedEffectId m_JobLedEffectId;
   /// Type of the new job
   LedEffectJobType m_JobType;
   /// Stop LED-effect sequence
   StopEffectSequence m_StopEffectSequence;
   /// Number of LEDs
   uint32_t m_NumberOfLeds;
   /// Instance of the running LED-effect
   CLedEffectBase * m_pRunningLedEffect;
   /// Task function of the LED-effect manager
   void TaskFct (void);
   /// LED-effect parameter
   uint32_t m_EffectParameter[MAX_NUMBER_OF_EFFECT_PARAMETER];
   /// Current frequency values 
   uint8_t m_FrequencyValues[7];
};

#endif //__LEDEFFECTMGR_H
