//-----------------------------------------------------------------------------
//  roselightapi.h
/// @file
///
/// @brief Application programmer interface for the RoseLightApi-protocol
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __ROSELIGHTAPI_H
 #define __ROSELIGHTAPI_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <stdint.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Definitions ,macros and constants
//-----------------------------------------------------------------------------
// Network ports
/// UDP device local port
#define UDP_DEVICE_LOCAL_PORT                6221
/// UDP device remote port
#define UDP_DEVICE_REMOTE_PORT               6221
/// UDP master local port
#define UDP_MASTER_LOCAL_PORT                6227
/// UDP master remote port
#define UDP_MASTER_REMOTE_PORT               UDP_DEVICE_LOCAL_PORT

/// Size of the message id
#define RLA_SIZE_OF_MSG_ID                   2

// Message id for roselight api commands
// UDP commands

/// Initial command to find any rose light in the system 
#define RLA_MSG_ID_IS_ANYONE_HERE            0x01
/// Basic device information of a rose light
#define RLA_MSG_ID_BASIC_DEVICE_INFO         0x02
/// Device name of a rose light 
#define RLA_MSG_ID_DEVICE_NAME               0x03
/// Request basic device information
#define RLA_MSG_ID_REQ_BASIC_DEVICE_INFO     0x04
/// Start led effect
#define RLA_MSG_ID_START_LED_EFFECT          0x05
/// Stop led effect
#define RLA_MSG_ID_STOP_LED_EFFECT           0x06

/// Frequency values changed
#define RLA_MSG_ID_FREQ_VALUE_CHANGED        0xA0
/// Set the color
#define RLA_MSG_ID_SET_COLOR                 0xA1

#pragma pack(1)

/// Data for the command RLA_MSG_ID_BASIC_DEVICE_INFO
typedef struct tagBASIC_DEVICE_INFO
{
   uint8_t     type;          ///< Type of the rose light device
   uint8_t     name[20];      ///< Name of the rose light device
   uint64_t    chipId;        ///< Chip id of the rose light device
   uint8_t     flags;         ///< Flags for the device
   uint8_t     version;       ///< Firmware version
   uint8_t     revision;      ///< Firmware revision
   uint8_t     subrevision;   ///< Firmware subrevision
} BASIC_DEVICE_INFO;

/// Data for the command RLA_MSG_ID_DEVICE_NAME
typedef struct tagDEVICE_NAME
{
   uint8_t  name[20];         ///< Name of the rose light device
} DEVICE_NAME;

/// Data for the command RLA_MSG_ID_START_LED_EFFECT
typedef struct tagSTART_LED_EFFECT
{
   uint16_t ledEffectId;      ///< LED-effect id
   uint32_t effectParams[20]; ///< LED-effect parameter
} START_LED_EFFECT;

/// Data for the command RLA_MSG_ID_STOP_LED_EFFECT
typedef struct tagSTOP_LED_EFFECT
{
   uint8_t stopEffectSequence;///< Stop sequence 
} STOP_LED_EFFECT;

/// Data for the command RLA_MSG_ID_FREQ_VALUES_CHANGED
typedef struct tagFREQ_VALUES_CHANGED
{
   uint8_t  Freq63HzValuePct;    ///< Frequency value 63 hz
   uint8_t  Freq160HzValuePct;   ///< Frequency value 160 hz
   uint8_t  Freq400HzValuePct;   ///< Frequency value 400 hz
   uint8_t  Freq1000HzValuePct;  ///< Frequency value 1000 hz
   uint8_t  Freq2500HzValuePct;  ///< Frequency value 2500 hz
   uint8_t  Freq6250HzValuePct;  ///< Frequency value 6250 hz
   uint8_t  Freq16000HzValuePct; ///< Frequency value 16000 hz
} FREQ_VALUES_CHANGED;

/// Data for the command RLA_MSG_ID_FREQ_VALUES_CHANGED
typedef struct tagSET_COLOR
{
   uint32_t  Color;           ///< Color value
} SET_COLOR;

/// Basis command definition for an api command
typedef struct tagRLA_MSG
{
   uint16_t cmdId;            ///< Id of the command

   /// Union for all message data
   typedef union tagDATA
   {
      BASIC_DEVICE_INFO    BasicDeviceInfo;  ///< Data for the message id RLA_MSG_ID_BASIC_DEVICE_INFO
      DEVICE_NAME          DeviceName;       ///< Data for the message id RLA_MSG_ID_DEVICE_NAME
      START_LED_EFFECT     StartLedEffect;   ///< Data for the message id RLA_MSG_ID_START_LED_EFFECT
      STOP_LED_EFFECT      StopLedEffect;    ///< Data for the message id RLA_MSG_ID_STOP_LED_EFFECT
      FREQ_VALUES_CHANGED  FreqValuesChanged;///< Data for the message id RLA_MSG_ID_FREQ_VALUE_CHANGED
   } DATA;
} RLA_MSG;

#pragma pack()

//-----------------------------------------------------------------------------
// Datatypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototypes of public functions
//-----------------------------------------------------------------------------

#endif //__ROSELIGHTAPI_H
