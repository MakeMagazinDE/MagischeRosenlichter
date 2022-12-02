//-----------------------------------------------------------------------------
//  effectstaticcolor.h
/// @file
///
/// @brief Header for the module effectstaticcolor.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTSINGLECOLOR_H
 #define __EFFECTSINGLECOLOR_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "ledeffectbase.h"

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Class CEffectSinglecolor
///
/// @brief Management class for the LED-effect 'Single color'
///
//-----------------------------------------------------------------------------
class CEffectSinglecolor : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CEffectSinglecolor();
   // Starts the LED-effect
   virtual void Start(void);
   // Gets the name of the effect
   const char * GetName(void);
private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Starts the LED-effect
   void StartEffect_SingleColor(void);
   // Sets the effect parameter
   void SetEffectParameter(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Effect brightness type 1
   uint32_t m_BrighnessType1Pct;
   /// Effect brightness type 2
   uint32_t m_BrighnessType2Pct;
   /// Color value
   uint32_t m_Color;
   /// Soft start time in ms
   uint32_t m_StartTimeMs;
   /// Flag: Device is master (true) or not (false)
   uint32_t m_IsMaster;
   /// Frequency for the master
   LedEffectFequency m_FrequencyMaster;
   /// Frequency for the slave
   LedEffectFequency m_FrequencySlave;
};

#endif //__EFFECTSINGLECOLOR_H
