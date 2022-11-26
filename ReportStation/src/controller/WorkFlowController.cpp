#include "controller/WorkFlowController.h"
#include "Arduino.h"
#include <WiFiClientSecure.h>

using namespace controller;

WorkFlowController::WorkFlowController(configuration::ConfigurationManager& config_manager,
									   communication::WiFiAggregator& wifi_aggregator,
									   data_generator::IDataGenerator* data_generator,
									   data::DataWrapper& data_storage)
	: config_manager(config_manager)
	, wifi_aggregator(wifi_aggregator)
	, data_generator(data_generator)
	, data_storage(data_storage)
	, last_latched_time(0)
{ }

void WorkFlowController::SendTemperatureToCloud()
{
	if(!data_storage.getAskSensorApi().isEmpty())
	{
		this->wifi_aggregator.StopSendingData();
		WiFiClientSecure client;
		String url;
		url += "/write/";
		url += data_storage.getAskSensorApi();
		url += "?module1=";
		url += data_storage.getCurrentTemperature();

		client.setInsecure();
		if(client.connect(definitions::ask_sensors_host, 443))
		{
			Serial.println("Client connected to: " + url);
			client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " +
						 definitions::ask_sensors_host + "\r\n" + "Connection: close\r\n\r\n");
			client.stop();
		}
		else
		{
			Serial.println("Client could not connect to: " + url);
		}
		this->wifi_aggregator.StartSendingData();
	}
}

void WorkFlowController::Service()
{
	auto config = config_manager.Get();
	auto time = millis();
	if(time - this->last_latched_time >=
	   static_cast<unsigned long>(config.report_peroid * 1000 * 60))
	{
		this->last_latched_time = time;
		uint16_t data_length =
			this->data_generator->GenerateData(this->data_buffer, sizeof(this->data_buffer));
		this->wifi_aggregator.SendData(this->data_buffer, data_length);
		this->SendTemperatureToCloud();

		// indicate that data were sent
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
		digitalWrite(LED_BUILTIN, HIGH);

		Serial.println("Data sent!");
		for(unsigned int i = 0; i < data_length; i++)
		{
			char buff[3];
			sprintf(buff, "%02X", this->data_buffer[i]);
			Serial.print(buff);
		}
		Serial.println("");
	}
}