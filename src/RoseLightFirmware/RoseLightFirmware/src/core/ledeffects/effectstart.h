//-----------------------------------------------------------------------------
//  effectstart.h
/// @file
///
/// @brief Header for the module effectstart.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTSTART_H
 #define __EFFECTSTART_H

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
//  Class CEffectStart
///
/// @brief Management class for the LED-effect 'Start'
///
//-----------------------------------------------------------------------------
class CEffectStart : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CEffectStart();
   // Sets the effect parameter and starts the effect
   virtual void Start(void);
   // Gets the name of the effect
   const char * GetName(void);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Sets the effect parameter
   void SetEffectParameter(void);
   // Starts the LED-effect
   void StartEffect_Start(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Effect brightness type 1
   uint32_t m_BrighnessType1Pct;
   /// Effect brightness type 2
   uint32_t m_BrighnessType2Pct;
   /// Color value
   uint32_t m_Color;
   /// Flag: Run forever (true) or not (false)
   uint32_t m_RunForever;
};

#endif //__EFFECTSTART_H
