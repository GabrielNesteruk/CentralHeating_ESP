#pragma once

#include "definitions.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <string.h>

namespace communication
{

class MQTT
{
private:
	WiFiClientSecure client;
	PubSubClient* mqtt_client;
	char broker[100];
	char topic[50];
	char mac_address[18];
	const int port;

public:
	MQTT(String mac_address);

	void Init();
	void Service();
	void SendData(uint8_t* buffer, size_t length);
};

} // namespace communication