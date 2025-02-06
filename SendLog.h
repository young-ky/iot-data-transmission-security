#ifndef SENDLOG_H
#define SENDLOG_H

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>

String sendLog(String token, int data);

#endif