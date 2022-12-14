#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationData.h"
#include "definitions.h"
#include <ArduinoJson.h>

using namespace communication;
using namespace configuration;

WiFiAggregator::WiFiAggregator(ESP8266WiFiClass& _WiFi,
							   configuration::ConfigurationManager& config_manager,
							   mqtt_topic::ITopicData<double>* topic_data,
							   controller::WorkFlowController<double>* work_flow_controller,
							   data::DataWrapper& data_storage,
							   lcd::Lcd& lcd,
							   device::PushButton& pushButton)
	: _WiFi(_WiFi)
	, config_manager(config_manager)
	, server{80}
	, basic_ip_address{192, 168, 1, 1}
	, basic_gateway_address{192, 168, 1, 1}
	, basic_mask{255, 255, 255, 0}
	, mqtt{WiFi.macAddress(), topic_data, work_flow_controller}
	, data_storage{data_storage}
	, work_flow_controller{work_flow_controller}
	, lcd{lcd}
	, pushButton{pushButton}
{
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
}

void WiFiAggregator::Init()
{
	bool is_config_in_memory = config_manager.Load();
	if(!is_config_in_memory)
	{
		lcd.Println(3, "Oczekiwanie", "na", "konfiguracje.");
		Serial.println("Config is no in memory");
		// there is no config in memory so were setting wifi as an AP and waiting
		// for a config data
		_WiFi.softAPConfig(basic_ip_address, basic_gateway_address, basic_mask);
		_WiFi.softAP(definitions::basic_station_name);
		WaitForConfigData();
	}
	else
	{
		Serial.println("Trying to connect!");
		// we have config in the memory so were going to connect to the network and set up listening mode
		auto config = config_manager.Get();
		if(static_cast<char>(config.ip[0]) != '\0')
		{
			_WiFi.config(IPAddress{config.ip}, IPAddress{config.gateway}, IPAddress{config.mask});
		}
		_WiFi.begin(reinterpret_cast<const char*>(config.ssid),
					reinterpret_cast<const char*>(config.password));

		auto capturedTime = millis();
		lcd.Print("WiFi Init...");
		while(WiFi.status() != WL_CONNECTED)
		{
			if(millis() - capturedTime >= static_cast<unsigned long>(1000 * 60 * 1))
			{
				lcd.PrintError(definitions::ERROR_TYPE::WIFI_CONNECTION_ERROR);
				while(true)
				{
					ESP.wdtFeed();
					pushButton.Service();
				}
			}
			Serial.println(".");
			delay(500);
		}

		Serial.println("WiFi connected.");
		Serial.println("IP Address:");
		Serial.println(_WiFi.localIP());
		_WiFi.setAutoReconnect(true);
		if(_WiFi.localIP().toString() != String{reinterpret_cast<const char*>(config.ip)})
		{
			_WiFi.localIP().toString().toCharArray(reinterpret_cast<char*>(&config.ip),
												   sizeof(config.ip));
			config_manager.Update(config);
		}
		ESP.wdtFeed();

		lcd.Print("MQTT Init...");
		delay(definitions::mqtt_broadcast_waiting_time *
			  1000); // wait for 20 seconds before publishing your ip
		ESP.wdtFeed();
		if(mqtt.PreInit(_WiFi))
		{
			Serial.println("Connected to MQTT broker.");
			mqtt.Init();
			SetServerEndpoints();
			server.begin();
		}
		else
		{
			// error !!!!
			Serial.println("Could not connect to MQTT broker.");
			lcd.PrintError(definitions::ERROR_TYPE::MQTT_CONNECTION_ERROR);
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
				this->config_manager.Update(read_config);
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

void WiFiAggregator::SetServerEndpoints()
{
	server.on("/clear", HTTP_POST, [this]() {
		this->server.send(200, "text/plain");
		this->config_manager.Clear();
		ESP.restart();
	});

	server.on("/alive", HTTP_GET, [this]() {
		DynamicJsonDocument doc(128);
		String json;

		doc["currentTemperature"] = serialized(String(this->data_storage.getAvgTemperature(), 1));
		doc["setTemperature"] = serialized(String(this->data_storage.getSetTemperature(), 1));
		serializeJson(doc, json);
		this->server.send(200, "application/json", json);
	});

	server.on("/changeTemperature", HTTP_POST, [this]() {
		String json = this->server.arg("plain");
		DynamicJsonDocument doc(128);
		DeserializationError error = deserializeJson(doc, json);
		if(!error)
		{
			this->data_storage.getSetTemperature() = doc["temperature"];
			this->work_flow_controller->UpdateInternalValues();
			Serial.println("Set temperature request, value: ");
			Serial.print(this->data_storage.getSetTemperature(), 2);
			Serial.println();
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
		lcd.PrintError(definitions::ERROR_TYPE::WIFI_CONNECTION_ERROR);

		while(WiFi.status() != WL_CONNECTED)
		{
			delay(500);
		}
		lcd.EnablePrinting();
	}

	if(!mqtt.IsConnected())
	{
		lcd.PrintError(definitions::ERROR_TYPE::MQTT_CONNECTION_ERROR);
		if(mqtt.RawConnect())
		{
			lcd.EnablePrinting();
		}
	}
	server.handleClient();
	mqtt.Service();
}
