//-----------------------------------------------------------------------------
//  effectmulticolor.h
/// @file
///
/// @brief Header for the module effectmulticolor.cpp
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __EFFECTMULTICOLOR_H
 #define __EFFECTMULTICOLOR_H

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
//  Class CEffectMulticolor
///
/// @brief Management class for the LED-effect 'Multi color'
///
//-----------------------------------------------------------------------------
class CEffectMulticolor : public CLedEffectBase
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CEffectMulticolor();
   // Gets the name of the effect
   const char * GetName(void);
   // Starts the effect
   virtual void Start(void);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Starts the effect 'MultiColor'
   void StartEffect_MultiColor(void);
   // Sets the effect parameter
   void SetEffectParameter(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Color value 1
   uint32_t m_Color1;
   /// Color value 2
   uint32_t m_Color2;
   /// Color value 3
   uint32_t m_Color3;
   /// Time to stay with the color
   uint32_t m_TimeColor;
   /// Time for switch between the colors
   uint32_t m_TimeSwitch;
   /// Effect brightness type 1
   uint32_t m_BrighnessType1Pct;
   /// Effect brightness type 2
   uint32_t m_BrighnessType2Pct;
};

#endif //__EFFECTMULTICOLOR_H
