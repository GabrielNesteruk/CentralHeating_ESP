#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationData.h"
#include "definitions.h"
#include <ArduinoJson.h>

using namespace communication;
using namespace configuration;

WiFiAggregator::WiFiAggregator(ESP8266WiFiClass& _WiFi,
							   configuration::ConfigurationManager& config_manager)
	: _WiFi(_WiFi)
	, config_manager(config_manager)
	, server{80}
	, basic_ip_address{192, 168, 1, 1}
	, basic_gateway_address{192, 168, 1, 1}
	, basic_mask{255, 255, 255, 0}
{
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
}

void WiFiAggregator::Init()
{
	bool is_config_in_memory = config_manager.Load();
	if(!is_config_in_memory)
	{
		// there is no config in memory so were setting wifi as an AP and waiting
		// for a config data
		_WiFi.softAPConfig(basic_ip_address, basic_gateway_address, basic_mask);
		_WiFi.softAP(definitions::basic_station_name);
		WaitForConfigData();
	}
	else
	{
		// we have config in the memory so were going to connect to the network and set up listening mode
		const auto& config = config_manager.GetCurrentBaseStationConfig();
		if(static_cast<char>(config.ip[0]) != '\0')
		{
			_WiFi.config(IPAddress{config.ip}, IPAddress{config.gateway}, IPAddress{config.mask});
		}
		_WiFi.begin(reinterpret_cast<const char*>(config.ssid),
					reinterpret_cast<const char*>(config.password));
		while(WiFi.status() != WL_CONNECTED)
		{
			delay(500);
		}

		Serial.println("WiFi connected.");
		Serial.println("IP Address:");
		Serial.println(_WiFi.localIP());
		if(_WiFi.localIP().toString() != String{reinterpret_cast<const char*>(config.ip)})
		{
			config_manager.SetBaseStationConfigIp(_WiFi.localIP());
			config_manager.Save();
		}
	}
}

void WiFiAggregator::WaitForConfigData()
{
	bool get_out_from_config = false;

	server.on("/", HTTP_POST, [this, &get_out_from_config]() {
		String json = this->server.arg("plain");
		DynamicJsonDocument doc(256);
		DeserializationError error = deserializeJson(doc, json);
		if(!error)
		{
			BaseStationConfiguration read_config;
			memcpy(read_config.ssid, doc["ssid"] | "", 32);
			memcpy(read_config.password, doc["password"] | "", 64);
			memcpy(read_config.mask, doc["mask"] | "", 16);
			memcpy(read_config.gateway, doc["gateway"] | "", 16);
			read_config.ip[0] = '\0';

			if(static_cast<char>(read_config.ssid[0]) == '\0' ||
			   static_cast<char>(read_config.password[0]) == '\0' ||
			   static_cast<char>(read_config.mask[0]) == '\0' ||
			   static_cast<char>(read_config.gateway[0]) == '\0')
			{
				this->server.send(404, "text/plain");
			}
			else
			{
				this->server.send(200, "text/plain");
				this->config_manager.SetBaseStationConfig(read_config);
				this->config_manager.Save();
				get_out_from_config = true;
			}
		}
		else
		{
			this->server.send(400, "text/plain");
		}
	});
	server.onNotFound([this]() { this->server.send(404, "text/plain"); });
	server.begin();

	while(!get_out_from_config)
	{
		server.handleClient();
	}

	ESP.restart();
}