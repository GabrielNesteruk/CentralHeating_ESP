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
	client.setInsecure();
	mqtt_client = new PubSubClient(client);
	mqtt_client->setServer(broker, port);
	if(mqtt_client->connect(
		   mac_address, definitions::broker_username, definitions::broker_password))
	{
		Serial.println("Connected to MQTT broker.");
		mqtt_client->subscribe(topic);
		mqtt_client->setCallback([this](char* topic, uint8_t* payload, unsigned int length) {
			// check if topic is correct, only then do something with data
			Serial.print("Message arrived [");
			Serial.print(topic);
			Serial.print("] ");
			Serial.println("");
			for(unsigned int i = 0; i < length; i++)
			{
				char buff[2];
				sprintf(buff, "%02X", payload[i]);
				Serial.print(buff);
			}
			Serial.println("");
			if(strcmp(this->topic, topic) == 0)
			{
				if(this->topic_data->SetPayload(payload, length))
				{
					this->work_flow_controller->UpdateReportStation(this->topic_data);
				}
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
	mqtt_client->loop();
}