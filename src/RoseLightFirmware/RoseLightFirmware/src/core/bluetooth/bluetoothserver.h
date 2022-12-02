//-----------------------------------------------------------------------------
//  bluetoothserver.h
/// @file
///
/// @brief Header for the module bluetoothserver.cpp
///
/// $Date: 2022-07-17 18:02:53 +0200 (So, 17 Jul 2022) $
/// $Revision: 156 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------
#ifndef __BLUETOOTHSERVER_H
 #define __BLUETOOTHSERVER_H

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <BLEDevice.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "task.h"

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
//  Class CRlBLEServerCallbacks
///
/// @brief Callback class for the BLE communication
///
/// @see CBluetoothServer
///
//-----------------------------------------------------------------------------
class CRlBLEServerCallbacks: public BLEServerCallbacks
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Callback for new connection to a bluetooth device
   void onConnect(BLEServer* pServer);
   // Callback for disconnection of a bluetooth device
   void onDisconnect(BLEServer* pServer);
};

//-----------------------------------------------------------------------------
//  Class CBluetoothServer
///
/// @brief Class managing the BLE communication to the Magic Rose Light devices
///
/// @see CRlBLEServerCallbacks
///
//-----------------------------------------------------------------------------
class CBluetoothServer : public CTask
{
public:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Constructor
   CBluetoothServer(void);
   // Elaborate method of the 'BleServer'-task
   void Elaborate (void);

private:
   //---------------------------------------------------------------------------
   // Member functions
   //---------------------------------------------------------------------------
   // Task function
   void TaskFct (void);
   // Starts the BLE server
   void StartServer(void);

   //---------------------------------------------------------------------------
   // Member variables                                       - short description
   //---------------------------------------------------------------------------
   /// Running BLE server instance
   BLEServer * m_pServer;
   /// BLE service
   BLEService * m_pService;
   /// Characteristic for set and get the SSID 
   BLECharacteristic * m_pCharacteristicSsid;
   /// Characteristic for set and get the WLAN password 
   BLECharacteristic * m_pCharacteristicPw;
   /// Characteristic for reset of the device 
   BLECharacteristic * m_pCharacteristicResetDevice;
   /// Characteristic for get the software version 
   BLECharacteristic * m_pCharacteristicSwVersion;
   /// Characteristic for get the hardware version 
   BLECharacteristic * m_pCharacteristicHwVersion;
   /// Characteristic for set and get the device name 
   BLECharacteristic * m_pCharacteristicName;
   /// Characteristic for get the device type 
   BLECharacteristic * m_pCharacteristicType;

   /// Current SSID value
   std::string * m_pSsid;
   /// Current password value
   std::string * m_pPw;
   /// Current device name value
   std::string * m_pName;
   /// Software version of the device
   uint32_t m_SwVersion;
   /// Hardware version of the device
   uint16_t m_HwVersion;
   /// Type of the device
   uint16_t m_Type;
   /// Boot mode (for the reset device procedure)
   uint8_t m_BootMode;
};

#endif //__BLUETOOTHSERVER_H
