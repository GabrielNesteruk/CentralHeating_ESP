#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationData.h"
#include "definitions.h"
#include <ArduinoJson.h>

using namespace communication;
using namespace configuration;

WiFiAggregator::WiFiAggregator(WiFiClass& _WiFi,
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

void WiFiAggregator::Init() { }

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
			read_config.ssid[sizeof(read_config.password) - 1] = '\0';
			memcpy(read_config.mask, doc["mask"] | "", 16);
			read_config.ssid[sizeof(read_config.mask) - 1] = '\0';
			memcpy(read_config.gateway, doc["gateway"] | "", 16);
			read_config.ssid[sizeof(read_config.gateway) - 1] = '\0';
			read_config.ip[0] = '\0';
			read_config.setpoint = definitions::default_setpoint;

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
		if(_WiFi.softAPgetStationNum())
		{
			digitalWrite(definitions::LED_BUILTIN, HIGH);
		}
		else
		{
			digitalWrite(definitions::LED_BUILTIN, LOW);
		}
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
			auto read_config = config_manager.Get();
			read_config.setpoint = this->data_storage.getSetTemperature();
			config_manager.Update(read_config);
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
	server.handleClient();
	delay(10);

	// if(!_WiFi.isConnected())
	// {
	// 	_WiFi.reconnect();
	// 	lcd.PrintError(definitions::ERROR_TYPE::WIFI_CONNECTION_ERROR);

	// 	while(WiFi.status() != WL_CONNECTED)
	// 	{
	// 		delay(500);
	// 	}
	// 	lcd.EnablePrinting();
	// }
	// else
	// {
	// 	server.handleClient();
	// 	delay(10);
	// }

	// if(!mqtt.IsConnected())
	// {
	// 	lcd.PrintError(definitions::ERROR_TYPE::MQTT_CONNECTION_ERROR);
	// 	if(mqtt.RawConnect())
	// 	{
	// 		lcd.EnablePrinting();
	// 	}
	// }
	// else
	// {
	// 	mqtt.Service();
	// }
}
