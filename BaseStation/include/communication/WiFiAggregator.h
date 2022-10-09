#pragma once

#include "IConfiguration.h"
#include "WorkFlowController.h"
#include "communication/MQTT.h"
#include "configuration/ConfigurationData.h"
#include "configuration/ConfigurationManager.h"
#include "definitions.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

namespace communication
{
class WiFiAggregator
{
private:
	ESP8266WiFiClass& _WiFi;
	configuration::ConfigurationManager& config_manager;
	ESP8266WebServer server;
	const IPAddress basic_ip_address; // basic ip address assigned for AP
	const IPAddress basic_gateway_address; // basic gateway address assigned for AP
	const IPAddress basic_mask; // basic mask assigned for AP
	MQTT mqtt;

	void WaitForConfigData();
	void SetServerEndpoints();

public:
	WiFiAggregator(ESP8266WiFiClass& _WiFi,
				   configuration::ConfigurationManager& config_manager,
				   mqtt_topic::ITopicData<double>* topic_data,
				   controller::WorkFlowController<double>* work_flow_controller);

	void Init();

	void Service();
};

} // namespace communication