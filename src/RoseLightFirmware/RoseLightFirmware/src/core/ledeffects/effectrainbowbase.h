//-----------------------------------------------------------------------------
//  effectrainbowbase.h
/// @file
///
/// @brief Header for the module effectrainbowbase.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTRAINBOWBASE_H
 #define __EFFECTRAINBOWBASE_H

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
/// Size for the color table
#define SIZEOFCOLORTABLE   360

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Class CEffectRainbowBase
///
/// @brief Base class for rainbow LED-effects
///
//-----------------------------------------------------------------------------
class CEffectRainbowBase : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CEffectRainbowBase(uint16_t EffectId);

protected:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Sets the effect parameter
   virtual void SetEffectParameter(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Effect brightness type 1
   uint32_t          m_BrighnessType1Pct;
   /// Effect brightness type 2
   uint32_t          m_BrighnessType2Pct;
   /// Flag: Device is master (true) or not (false)
   uint32_t          m_IsMaster;
   /// Frequency for the master
   LedEffectFequency m_FrequencyMaster;
   /// Frequency for the slave
   LedEffectFequency m_FrequencySlave;

   /// Color table for the rainbow
   uint32_t RingColors[SIZEOFCOLORTABLE];  

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Builds the color table for the rainbow
   void BuildColorTable(void);
};

#endif //__EFFECTRAINBOWBASE_H
