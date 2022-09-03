#include "communication/DefaultTopicDataParser.h"

using namespace mqtt_topic;

void DefaultTopicDataParser::SetPayload(uint8_t* payload, unsigned int length)
{
	if(length > sizeof(this->payload))
	{
		length = sizeof(this->payload);
	}

	memcpy(this->payload, payload, length);
}

double DefaultTopicDataParser::GetValue()
{
	return (reinterpret_cast<TopicData*>(this->payload))->temperature;
}

const char* DefaultTopicDataParser::GetName()
{
	auto ptr = reinterpret_cast<TopicData*>(this->payload)->name;
	return reinterpret_cast<const char*>(ptr);
}

uint8_t DefaultTopicDataParser::GetId()
{
	return reinterpret_cast<TopicData*>(this->payload)->id;
}

uint8_t DefaultTopicDataParser::GetReportPeroid()
{
	return reinterpret_cast<TopicData*>(this->payload)->report_peroid;
}