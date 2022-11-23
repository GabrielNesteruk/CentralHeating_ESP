#pragma once

#include "definitions.h"
#include <ITopicData.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WorkFlowController.h>
#include <assert.h>
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
	char raw_buffer[20];
	const int port;
	mqtt_topic::ITopicData<double>* topic_data;
	controller::WorkFlowController<double>* work_flow_controller;

public:
	MQTT(String mac_address,
		 mqtt_topic::ITopicData<double>* topic_data,
		 controller::WorkFlowController<double>* work_flow_controller);

	bool PreInit(WiFiClass& _WiFi);
	void Init();
	void Service();
	bool RawConnect();
	bool IsConnected()
	{
		if(mqtt_client != nullptr)
			return mqtt_client->connected();
		else
			return false;
	}
	void Disconnect()
	{
		mqtt_client->disconnect();
	}
};

} // namespace communication