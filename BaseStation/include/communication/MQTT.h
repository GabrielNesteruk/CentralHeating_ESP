#pragma once

#include "definitions.h"
#include <ESP8266WiFi.h>
#include <ITopicData.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WorkFlowController.h>
#include <assert.h>
#include <string.h>

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
	mqtt_topic::ITopicData<double>* topic_data;
	controller::WorkFlowController<double>* work_flow_controller;

public:
	MQTT(String mac_address,
		 mqtt_topic::ITopicData<double>* topic_data,
		 controller::WorkFlowController<double>* work_flow_controller);

	void Init();
	void Service();
};

} // namespace communication