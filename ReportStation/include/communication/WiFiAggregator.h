#pragma once

#include "IConfiguration.h"
#include "communication/MQTT.h"
#include "configuration/ConfigurationData.h"
#include "configuration/ConfigurationManager.h"
#include "definitions.h"
#include "device/PushButton.h"
#include <ArduinoJson.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <misc/DataWrapper.h>

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
	data::DataWrapper& data_storage;
	device::PushButton& pushButton;

	void WaitForConfigData();

public:
	WiFiAggregator(ESP8266WiFiClass& _WiFi,
				   configuration::ConfigurationManager& config_manager,
				   data::DataWrapper& data_storage,
				   device::PushButton& pushButton);

	void Init();

	void Service();

	void SendData(uint8_t* buffer, size_t length);

	void SetServerEndpoints();
};

} // namespace communication