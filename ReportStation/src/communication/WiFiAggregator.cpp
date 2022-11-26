#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationData.h"
#include "definitions.h"
#include <ArduinoJson.h>

using namespace communication;
using namespace configuration;

WiFiAggregator::WiFiAggregator(ESP8266WiFiClass& _WiFi,
							   configuration::ConfigurationManager& config_manager,
							   data::DataWrapper& data_storage,
							   device::PushButton& pushButton)
	: _WiFi(_WiFi)
	, config_manager(config_manager)
	, server{80}
	, basic_ip_address{192, 168, 1, 1}
	, basic_gateway_address{192, 168, 1, 1}
	, basic_mask{255, 255, 255, 0}
	, mqtt{WiFi.macAddress()}
	, data_storage{data_storage}
	, pushButton{pushButton}
{
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
}

void WiFiAggregator::ExtractIpOctetsFromString(const char* str, uint8_t* array)
{
	char tmp_buff[4];
	for(size_t i{}, k{}, l{}; i < strlen(str); i++)
	{
		char character = str[i];
		if(i == strlen(str) - 1)
		{
			tmp_buff[l++] = character;
			tmp_buff[l] = '\0';
			array[k] = static_cast<uint8_t>(atoi(tmp_buff));
		}
		else if(character != '.')
		{
			tmp_buff[l++] = character;
		}
		else
		{
			tmp_buff[l] = '\0';
			array[k++] = static_cast<uint8_t>(atoi(tmp_buff));
			l = 0;
		}
	}
}

void WiFiAggregator::Init()
{
	bool is_config_in_memory = config_manager.Load();
	if(!is_config_in_memory)
	{
		Serial.println("Config is no in memory");
		// there is no config in memory so were setting wifi as an AP and waiting
		// for a config data
		_WiFi.softAPConfig(basic_ip_address, basic_gateway_address, basic_mask);
		char tmpSSID[100];
		sprintf(tmpSSID, "%s %d", definitions::default_report_station_name, ESP.getChipId());
		_WiFi.softAP(tmpSSID);
		WaitForConfigData();
	}
	else
	{
		Serial.println("Trying to connect!");
		// we have config in the memory so were going to connect to the network and set up listening mode
		auto config = config_manager.Get();
		if(static_cast<char>(config.ip[0]) != '\0')
		{
			IPAddress ipAdresses[3];
			uint8_t octets[4];

			ExtractIpOctetsFromString((char*)config.ip, octets);
			ipAdresses[0] = IPAddress{octets[0], octets[1], octets[2], octets[3]};
			ExtractIpOctetsFromString((char*)config.gateway, octets);
			ipAdresses[1] = IPAddress{octets[0], octets[1], octets[2], octets[3]};
			ExtractIpOctetsFromString((char*)config.mask, octets);
			ipAdresses[2] = IPAddress{octets[0], octets[1], octets[2], octets[3]};

			// there is a problem with DNS when trying to connect to the MQTT broker ;/
			// _WiFi.config(ipAdresses[0], ipAdresses[1], ipAdresses[2]);
		}
		_WiFi.begin(reinterpret_cast<const char*>(config.ssid),
					reinterpret_cast<const char*>(config.password));

		while(WiFi.status() != WL_CONNECTED)
		{
			pushButton.Service();
			Serial.println(".");
			delay(500);
		}

		Serial.println("WiFi connected.");
		Serial.println("IP Address:");
		Serial.println(_WiFi.localIP());
		_WiFi.setAutoReconnect(true);
		if(_WiFi.localIP().toString() != String{reinterpret_cast<const char*>(config.ip)})
		{
			_WiFi.localIP().toString().toCharArray(reinterpret_cast<char*>(config.ip),
												   sizeof(config.ip));
			config_manager.Update(config);
		}

		delay(definitions::mqtt_broadcast_waiting_time *
			  1000); // wait for 20 seconds before publishing your ip
		if(mqtt.PreInit(_WiFi))
		{
			Serial.println("Connected to MQTT broker.");
			SetServerEndpoints();
			server.begin();
			// indicate that everything is ok
			for(size_t i{}; i < 3; i++)
			{
				digitalWrite(LED_BUILTIN, LOW);
				delay(300);
				digitalWrite(LED_BUILTIN, HIGH);
				delay(200);
			}
		}
		else
		{
			// error !!!!
			Serial.println("Could not connect to MQTT broker.");
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
			auto read_config = config_manager.Get();
			memcpy(read_config.ssid, doc["ssid"] | "", 32);
			read_config.ssid[sizeof(read_config.ssid) - 1] = '\0';
			memcpy(read_config.password, doc["password"] | "", 64);
			read_config.password[sizeof(read_config.password) - 1] = '\0';
			memcpy(read_config.mask, doc["mask"] | "", 16);
			read_config.mask[sizeof(read_config.mask) - 1] = '\0';
			memcpy(read_config.gateway, doc["gateway"] | "", 16);
			read_config.gateway[sizeof(read_config.gateway) - 1] = '\0';
			memcpy(read_config.name, doc["name"] | "", definitions::max_report_station_name_length);
			read_config.name[definitions::max_report_station_name_length] = '\0';
			read_config.uid = doc["uid"] | 0xFF;
			read_config.report_peroid = doc["peroid"] | 15;
			read_config.ip[0] = '\0';

			if(static_cast<char>(read_config.ssid[0]) == '\0' ||
			   static_cast<char>(read_config.password[0]) == '\0' ||
			   static_cast<char>(read_config.mask[0]) == '\0' ||
			   static_cast<char>(read_config.gateway[0]) == '\0' ||
			   static_cast<char>(read_config.name[0]) == '\0' || read_config.uid == 0xFF)
			{
				this->server.send(404, "text/plain", "Data can not be empty!");
			}
			else
			{
				this->server.send(200, "text/plain");
				memset(read_config.api_key, 0x00, sizeof(read_config.api_key));
				this->config_manager.Update(read_config);
				get_out_from_config = true;
			}
		}
		else
		{
			this->server.send(400, "text/plain");
		}
	});
	server.onNotFound([this]() { this->server.send(404, "text/plain", "Request not found!"); });
	server.begin();

	while(!get_out_from_config)
	{
		if(_WiFi.softAPgetStationNum())
		{
			digitalWrite(LED_BUILTIN, LOW);
		}
		else
		{
			digitalWrite(LED_BUILTIN, HIGH);
		}
		server.handleClient();
	}

	ESP.restart();
}

void WiFiAggregator::SendData(uint8_t* buffer, size_t length)
{
	mqtt.SendData(buffer, length);
}

void WiFiAggregator::SetServerEndpoints()
{
	server.on("/clear", HTTP_POST, [this]() {
		this->server.send(200, "text/plain");
		this->config_manager.Clear();
		ESP.restart();
	});

	server.on("/alive", HTTP_GET, [this]() {
		DynamicJsonDocument doc(256);
		String json;
		auto read_config = config_manager.Get();

		doc["uid"] = read_config.uid;
		doc["name"] = read_config.name;
		doc["currentTemperature"] =
			serialized(String(this->data_storage.getCurrentTemperature(), 1));
		doc["humidity"] = serialized(String(this->data_storage.getHumidity(), 1));
		doc["peroid"] = read_config.report_peroid;
		serializeJson(doc, json);
		this->server.send(200, "application/json", json);
	});

	server.on("/settings", HTTP_POST, [this]() {
		String json = this->server.arg("plain");
		DynamicJsonDocument doc(256);
		DeserializationError error = deserializeJson(doc, json);
		if(!error)
		{
			auto read_config = config_manager.Get();
			memcpy(read_config.name, doc["name"] | "", definitions::max_report_station_name_length);
			read_config.report_peroid = doc["peroid"] | read_config.report_peroid;
			this->config_manager.Update(read_config);
			this->server.send(200, "text/plain");
		}
		else
		{
			this->server.send(400, "text/plain");
		}
	});

	server.on("/setAskSensorApi", HTTP_POST, [this]() {
		String json = this->server.arg("plain");
		DynamicJsonDocument doc(256);
		DeserializationError error = deserializeJson(doc, json);
		if(!error)
		{
			this->data_storage.getAskSensorApi() = doc["api"] | "";
			if(this->data_storage.getAskSensorApi() != "")
			{
				auto config = this->config_manager.Get();
				memcpy(config.api_key, doc["api"] | "", definitions::ask_sensors_api_key_length);
				config.api_key[sizeof(config.api_key) - 1] = '\0';
				Serial.println("API SAVED: ");
				Serial.print((char*)config.api_key);
				Serial.println();
				this->config_manager.Update(config);
			}
			this->server.send(200, "text/plain");
		}
		else
		{
			this->server.send(400, "text/plain");
		}
	});
}

void WiFiAggregator::Service()
{
	if(!_WiFi.isConnected())
	{
		_WiFi.reconnect();
		while(WiFi.status() != WL_CONNECTED)
		{
			digitalWrite(LED_BUILTIN, LOW);
			delay(500);
			digitalWrite(LED_BUILTIN, HIGH);
			delay(200);
			Serial.println("Trying to reconnect to WiFi Network...");
		}
		Serial.println("WiFi reconnected!!!");
	}
	else
	{
		server.handleClient();
	}

	if(!mqtt.IsConnected())
	{
		Serial.println("Trying to reconnect to MQTT Broker...");
		digitalWrite(LED_BUILTIN, LOW);
		if(mqtt.RawConnect())
		{
			Serial.println("MQTT Broker reconnected!!!");
			digitalWrite(LED_BUILTIN, HIGH);
		}
	}
	else
	{
		mqtt.Service();
	}
}
