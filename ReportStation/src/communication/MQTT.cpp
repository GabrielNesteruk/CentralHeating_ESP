#include "communication/MQTT.h"
#include "definitions.h"
#include <string.h>

using namespace communication;

MQTT::MQTT(String mac_address)
	: client()
	, port{definitions::broker_port}
{
	if((sizeof(definitions::broker_address) < sizeof(this->broker)) &&
	   (sizeof(definitions::topic_name) < sizeof(this->topic)))
	{
		strcpy(this->broker, definitions::broker_address);
		strcpy(this->topic, definitions::topic_name);
	}

	mac_address.toCharArray(this->mac_address, sizeof(this->mac_address));
}

bool MQTT::PreInit(ESP8266WiFiClass& _WiFi)
{
	client.setInsecure();
	mqtt_client = new PubSubClient(client);
	mqtt_client->setServer(broker, port);
	bool isConnected = (mqtt_client->connect(
		mac_address, definitions::broker_username, definitions::broker_password));

	if(isConnected)
	{
		_WiFi.localIP().toString().toCharArray(this->raw_buffer, sizeof(this->raw_buffer));
		Serial.println("Publishing: ");
		Serial.print(this->raw_buffer);
		Serial.println();
		mqtt_client->publish(definitions::topic_ip_broadcast, this->raw_buffer),
			strlen(this->raw_buffer);
		return true;
	}
	else
	{
		return false;
	}
}

void MQTT::SendData(uint8_t* buffer, size_t length)
{
	mqtt_client->publish(this->topic, buffer, length);
}

void MQTT::Service()
{
	mqtt_client->loop();
}