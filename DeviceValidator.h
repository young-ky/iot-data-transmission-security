#ifndef DEVICEVALIDATOR_H
#define DEVICEVALIDATOR_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

String validateDevice(String deviceIdentifier, String devicePassword);

#endif