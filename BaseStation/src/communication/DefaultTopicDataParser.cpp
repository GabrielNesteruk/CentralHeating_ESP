#include "communication/DefaultTopicDataParser.h"
#include <stddef.h>

using namespace mqtt_topic;

DefaultTopicDataParser::DefaultTopicDataParser() { }

void DefaultTopicDataParser::SetPayload(uint8_t* payload, unsigned int length)
{
	if(length > sizeof(this->payload))
	{
		length = sizeof(this->payload);
	}

	for(size_t i{}; i < length; i++)
	{
		this->payload[i] = payload[i];
	}
}

double DefaultTopicDataParser::GetTemperature()
{
	return 0.0;
}

const char* DefaultTopicDataParser::GetName()
{
	return this->name;
}

uint8_t DefaultTopicDataParser::GetId()
{
	return 0;
}

uint8_t DefaultTopicDataParser::GetReportPeroid()
{
	return 0;
}