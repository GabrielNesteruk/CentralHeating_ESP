#pragma once

#include "ITopicData.h"
#include "configuration/ConfigurationData.h"
#include "definitions.h"
#include <stddef.h>
#include <string.h>

namespace mqtt_topic
{

class DefaultTopicDataParser : public ITopicData<double>
{
private:
	struct TopicData
	{
		uint8_t id;
		uint8_t name[definitions::max_report_station_name_length];
		uint8_t report_peroid;
		double temperature;
		uint16_t crc;
	};

	uint8_t payload[100];

public:
	virtual void SetPayload(uint8_t* payload, unsigned int length) override;
	virtual double GetValue() override;
	virtual const char* GetName() override;
	virtual uint8_t GetId() override;
	virtual uint8_t GetReportPeroid() override;
};

} // namespace mqtt_topic