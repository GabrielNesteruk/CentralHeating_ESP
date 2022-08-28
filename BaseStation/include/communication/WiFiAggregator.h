#pragma once

#include "configuration/ConfigurationManager.h"
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

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

	void WaitForConfigData();

public:
	WiFiAggregator(ESP8266WiFiClass& _WiFi, configuration::ConfigurationManager& config_manager);
	void Init();
};

} // namespace communication