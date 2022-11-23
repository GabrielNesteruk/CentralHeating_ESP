#include "misc/AskStationsCloud.h"
#include "definitions.h"
#include <Arduino.h>
#include <WiFiClientSecure.h>

using namespace misc;

AskStationsCloud::AskStationsCloud(data::DataWrapper& data_storage,
								   communication::WiFiAggregator& wifi_aggregator)
	: data_storage{data_storage}
	, wifi_aggregator{wifi_aggregator}
	, send_data_flag{false}
	, value{false}
{ }

void AskStationsCloud::Service()
{
	if(this->send_data_flag && !data_storage.getAskSensorApi().isEmpty())
	{
		this->send_data_flag = false;

		wifi_aggregator.StopSendingData();

		WiFiClientSecure client;
		String url;
		url += "/write/";
		url += data_storage.getAskSensorApi();
		url += "?module1=";
		url += this->value;

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
		};

		wifi_aggregator.StartSendingData();
	}
}

void AskStationsCloud::SetValueToSend(bool value)
{
	this->send_data_flag = true;
	this->value = value;
}