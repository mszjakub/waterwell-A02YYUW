// Code generated by Arduino IoT Cloud, DO NOT EDIT.

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "264e6acb-2a86-4a57-8fcc-f9bf113f5486";

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_OPTIONAL_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onLedChange();

CloudLight led;
CloudLength distance;
CloudLength waterLevel;
CloudVolume waterVolume;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.addProperty(led, READWRITE, ON_CHANGE, onLedChange);
  ArduinoCloud.addProperty(distance, READ, ON_CHANGE, NULL, 1);
  ArduinoCloud.addProperty(waterLevel, READ, ON_CHANGE, NULL, 2);
  ArduinoCloud.addProperty(waterVolume, READ, ON_CHANGE, NULL, 1);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);