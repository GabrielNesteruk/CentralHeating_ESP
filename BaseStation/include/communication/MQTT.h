#pragma once

#include "communication/ITopicData.h"
#include "definitions.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

namespace communication
{

class MQTT
{
private:
	WiFiClient client;
	PubSubClient mqtt_client;
	char broker[40];
	char topic[50];
	char mac_address[18];
	const int port;
	mqtt_topic::ITopicData* topic_data;

public:
	MQTT(String mac_address, mqtt_topic::ITopicData* topic_data);
	void Init();
	void Service();
};

} // namespace communication