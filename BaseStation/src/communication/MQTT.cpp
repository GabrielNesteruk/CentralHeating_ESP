#include "communication/MQTT.h"
#include "definitions.h"
#include <assert.h>
#include <string.h>

using namespace communication;
using namespace mqtt_topic;

MQTT::MQTT(String mac_address,
		   ITopicData<double>* topic_data,
		   controller::WorkFlowController<double>* work_flow_controller)
	: client()
	, mqtt_client{client}
	, port{definitions::broker_port}
	, topic_data{topic_data}
	, work_flow_controller{work_flow_controller}
{
	if((sizeof(definitions::broker_address) < sizeof(this->broker)) &&
	   (sizeof(definitions::topic_name) < sizeof(this->topic)))
	{
		strcpy(this->broker, definitions::broker_address);
		strcpy(this->topic, definitions::topic_name);
	}

	mac_address.toCharArray(this->mac_address, sizeof(this->mac_address));
}

void MQTT::Init()
{
	mqtt_client.setServer(broker, port);
	if(mqtt_client.connect(mac_address))
	{
		Serial.println("Connected to MQTT broker.");
		mqtt_client.subscribe(topic);
		mqtt_client.setCallback([this](char* topic, uint8_t* payload, unsigned int length) {
			// check if topic is correct, only then do something with data
			Serial.print("Message arrived [");
			Serial.print(topic);
			Serial.print("] ");
			if(strcmp(this->topic, topic) == 0)
			{
				Serial.println("Payload Added");
				// Serial.print("Message arrived [");
				// Serial.print(topic);
				// Serial.print("] ");

				// for(unsigned int i = 0; i < length; i++)
				// {
				// 	Serial.print((char)payload[i]);
				// }

				this->topic_data->SetPayload(payload, length);
				this->work_flow_controller->UpdateReportStation(this->topic_data);
			}
		});
	}
	else
	{
		Serial.println("Could not connect to MQTT broker.");
	}
}

void MQTT::Service()
{
	mqtt_client.loop();
}