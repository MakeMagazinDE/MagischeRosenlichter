//-----------------------------------------------------------------------------
//  bluetoothserver.cpp
/// @file
///
/// @brief Implementation of the classes CRlBLEServerCallbacks and CBluetoothServer
///
/// $Date: 2022-08-26 17:16:24 +0200 (Fr, 26 Aug 2022) $
/// $Revision: 171 $
/// $LastChangedBy: EMueller $
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Standard include files
//-----------------------------------------------------------------------------
#include <BLEUtils.h>
#include <BLEServer.h>

//-----------------------------------------------------------------------------
// Project specific include files
//-----------------------------------------------------------------------------
#include "bluetoothserver.h"
#include "AppPreferences.h"
#include "version.h"
#include "hardware.h"

//-----------------------------------------------------------------------------
// Privat definitions, macros and constants
//-----------------------------------------------------------------------------
/// UUID for 'Service'-service
#define SERVICE_UUID                      "7C28CBCC-3C94-4371-B92A-682B49B3FE55"
/// UUID for 'SSID'-characteristic
#define CHARACTERISTIC_SSID_UUID          "CFBA3108-7817-4C45-A346-FCEE0D73F7E6"
/// UUID for 'Password'-characteristic
#define CHARACTERISTIC_PW_UUID            "BFDC68C0-35D8-4079-B052-2D2477FAB6F1"
/// UUID for 'Device reset'-characteristic
#define CHARACTERISTIC_DEVICE_RESET_UUID  "0B49FE53-43FB-4EAD-86C5-53B3A9AA83BC"
/// UUID for 'Software version'-characteristic
#define CHARACTERISTIC_SW_VERSION_UUID    "03E43E56-B198-4912-8F83-CCD68593C043"
/// UUID for 'Hardware version'-characteristic
#define CHARACTERISTIC_HW_VERSION_UUID    "05DF9A9D-8733-4263-8928-6706991DBF5C"
/// UUID for 'Device type'-characteristic
#define CHARACTERISTIC_TYPE_UUID          "98DDAF04-CC14-4B19-B8D2-C91D761C2348"
/// UUID for 'Device name'-characteristic
#define CHARACTERISTIC_NAME_UUID          "B298D9EB-C650-4D56-A6B1-F36CD04846EF"

//-----------------------------------------------------------------------------
// Private data types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External references
//-----------------------------------------------------------------------------
extern CHardware Hardware;

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
//  Method onConnect
///
/// @brief Callback for new connection to a bluetooth device
///
/// @param[in] pServer - BLE server instance
///
/// @see onDisconnect
///
//-----------------------------------------------------------------------------
void CRlBLEServerCallbacks::onConnect(BLEServer* pServer)
{
   Serial.println("onConnect");

   if (NULL != pServer)
   {
      Serial.println("startAdvertising");
      pServer->startAdvertising();
   }
};

//-----------------------------------------------------------------------------
//  Method onDisconnect
///
/// @brief Callback for disconnection of a bluetooth device
///
/// @param[in] pServer - BLE server instance
///
/// @see onConnect()
///
//-----------------------------------------------------------------------------
void CRlBLEServerCallbacks::onDisconnect(BLEServer* pServer)
{
   Serial.println("onDisconnect");
}

/*---------------------------------------------*/

//-----------------------------------------------------------------------------
//  Method CBluetoothServer
///
/// @brief Constructor of the class CBluetoothServer
///
//-----------------------------------------------------------------------------
CBluetoothServer::CBluetoothServer(void)
{
}

//-----------------------------------------------------------------------------
//  Method Elaborate
///
/// @brief Elaborate method of the 'BleServer'-task
///
/// @see TaskFct()
///
//-----------------------------------------------------------------------------
void CBluetoothServer::Elaborate (void)
{
   SetName("tBleServer");

   SetStackSize(8192);
   SetPriority(2);
   Startup(0);
}

//-----------------------------------------------------------------------------
//  Method TaskFct
///
/// @brief Task function method of the 'BleServer'-task
///
/// @see Elaborate()
///
//-----------------------------------------------------------------------------
void CBluetoothServer::TaskFct (void)
{
   StartServer();

   for(;;)
   {
      CTask::Sleep(200);

      std::string ssid = m_pCharacteristicSsid->getValue();
      std::string pw = m_pCharacteristicPw->getValue();
      std::string name = m_pCharacteristicName->getValue();
      uint8_t * pResetDeviceData = m_pCharacteristicResetDevice->getData();

      if (0 != ssid.compare(*m_pSsid))
      {
         Serial.printf("Ssid id changed to: %s\n", (ssid.c_str()));

         AppPreferences::SetSSID((char *)(ssid.c_str()));
         *m_pSsid = ssid;
      }
      if (0 != pw.compare(*m_pPw))
      {
         Serial.printf("Pw id changed to: %s\n", (pw.c_str()));

         AppPreferences::SetPassword((char *)(pw.c_str()));
         *m_pPw = pw;
      }
      if (0 != name.compare(*m_pName))
      {
         Serial.printf("Name id changed to: %s\n", (name.c_str()));

         AppPreferences::SetName((char *)(name.c_str()));
         *m_pName = name;
      }
      if (1 != pResetDeviceData[0])
      {
         Serial.printf("Reset device %i\n", pResetDeviceData[0]);
         ESP.restart();
      }
   }
}

//-----------------------------------------------------------------------------
//  Method StartServer
///
/// @brief Starts the BLE server
///
//-----------------------------------------------------------------------------
void CBluetoothServer::StartServer(void)
{
   Serial.println("Starting BLE Server!");

   BLEDevice::init("RoseLight-BLE-Server");

   Serial.println("BLE server is init!");

   // create the server ...
   m_pServer = BLEDevice::createServer();

   // and the central service
   m_pService = m_pServer->createService(SERVICE_UUID);

   // create the 'ssid'-characteristic
   m_pCharacteristicSsid = m_pService->createCharacteristic(
                                          CHARACTERISTIC_SSID_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                       );

   m_pSsid = new std::string(AppPreferences::GetSSID().c_str());
   m_pCharacteristicSsid->setValue(*m_pSsid);

   // create the 'password'-characteristic
   m_pCharacteristicPw = m_pService->createCharacteristic(
                                          CHARACTERISTIC_PW_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                       );

   m_pPw = new std::string(AppPreferences::GetPassword().c_str());
   m_pCharacteristicPw->setValue(*m_pPw);

   // create 'software version'-characteristic
   m_pCharacteristicSwVersion = m_pService->createCharacteristic(
                                          CHARACTERISTIC_SW_VERSION_UUID,
                                          BLECharacteristic::PROPERTY_READ
                                       );

   m_SwVersion = VERSION_UINT32;
   m_pCharacteristicSwVersion->setValue(m_SwVersion);

   // create 'hardware version'-characteristic
   m_pCharacteristicHwVersion = m_pService->createCharacteristic(
                                          CHARACTERISTIC_HW_VERSION_UUID,
                                          BLECharacteristic::PROPERTY_READ
                                       );

   m_HwVersion = Hardware.GetHardwareVersion();
   m_pCharacteristicHwVersion->setValue(m_HwVersion);

   // create 'type'-characteristic
   m_pCharacteristicType = m_pService->createCharacteristic(
                                          CHARACTERISTIC_TYPE_UUID,
                                          BLECharacteristic::PROPERTY_READ
                                       );

   m_Type = (uint16_t)AppPreferences::GetType();
   m_pCharacteristicType->setValue(m_Type);

   // create the 'name'-characteristic
   m_pCharacteristicName = m_pService->createCharacteristic(
                                          CHARACTERISTIC_NAME_UUID,
                                          BLECharacteristic::PROPERTY_READ |
                                          BLECharacteristic::PROPERTY_WRITE
                                       );

   m_pName = new std::string(AppPreferences::GetName().c_str());
   m_pCharacteristicName->setValue(*m_pName);

   // create 'reset device'-characteristic
   m_pCharacteristicResetDevice = m_pService->createCharacteristic(
                                          CHARACTERISTIC_DEVICE_RESET_UUID,
                                          BLECharacteristic::PROPERTY_WRITE
                                       );

   m_BootMode = 1;
   m_pCharacteristicResetDevice->setValue(&m_BootMode, 1);

   // start the central service
   m_pService->start();
   BLEAdvertising *pAdvertising =m_pServer->getAdvertising();

   pAdvertising->addServiceUUID(SERVICE_UUID);
   pAdvertising->setScanResponse(true);
   pAdvertising->setAdvertisementType(ADV_TYPE_IND);
   pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue

   BLEAdvertisementData advData;

   advData.setName(AppPreferences::GetName().c_str());
   advData.setManufacturerData("TABO");
   pAdvertising->setAdvertisementData(advData);

   m_pServer->setCallbacks(new CRlBLEServerCallbacks());

   Serial.println("Start advertising ...");

   m_pServer->startAdvertising();

   Serial.printf("BLE address: %s\r\n", BLEDevice::getAddress().toString().c_str());

   Serial.println("Characteristic defined! Now you can read and write it in the Client!");
}
