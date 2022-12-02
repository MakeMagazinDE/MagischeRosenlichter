//-----------------------------------------------------------------------------
//  ledeffectdefs.h
/// @file
///
/// @brief Contains the basic LED-effect definitions
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __LEDEFFECTDEFS_H
 #define __LEDEFFECTDEFS_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------
/// Start type of the system
enum class SystemStartType
{
   Unknown = 0,            ///< Unknown mode
   Normal,                 ///< Normal system mode
   ConfigMode,             ///< Configuration mode
   Error                   ///< Error mode
};

/// Available LED-effect id of the system
enum class LedEffectId
{
   Unknown = 0,            ///< Unknown LED-effect id
   MultiColor,             ///< Effect id: Multi color
   SingleColor,            ///< Effect id: Single color
   Rainbow,                ///< Effect id: Rainbow
   Rainbow2,               ///< Effect id: Rainbow 2
   Fire,                   ///< Effect id: Fire
   SnowFlake,              ///< Effect id: Snow flake
   SnowGlobe,              ///< Effect id: Snow globe
   Heart,                  ///< Effect id: Heart
   ChangingColor,          ///< Effect id: Changing color
   Start                   ///< Effect id: Start
};

/// Sequences for the 'Stop'-sequence of an LED-effect
enum class StopEffectSequence
{
   Unknown = 0,            ///< Unknown sequence
   HardSwitchOff,          ///< Hard switch off sequence
   SoftSwitchOff           ///< Soft switch off sequence (fade out)
};

/// Led effect frequencies
enum class LedEffectFequency
{
   Unknown = 0,            ///< Unknown frequency / Invalid value
   Freq63Hz,               ///< Frequency 63 Hz
   Freq160Hz,              ///< Frequency 160 Hz
   Freq400Hz,              ///< Frequency 400 Hz
   Freq1000Hz,             ///< Frequency 1000 Hz
   Freq2500Hz,             ///< Frequency 2500 Hz
   Freq6250Hz,             ///< Frequency 6250 Hz
   Freq16000Hz,            ///< Frequency 16000 Hz
   AllFrequencies,         ///< All Frequencies
   Off                     ///< Off (no frequency)
};

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

#endif //__LEDEFFECTDEFS_H
