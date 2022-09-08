#pragma once

#include "ITopicData.h"
#include "configuration/ConfigurationData.h"
#include "definitions.h"
#include <Arduino.h>
#include <stddef.h>
#include <string.h>

namespace mqtt_topic
{

class DefaultTopicDataParser : public ITopicData<double>
{
private:
	const uint8_t id_offset = 0;
	const uint8_t name_offset = id_offset + sizeof(uint8_t);
	const uint8_t report_peroid_offset =
		name_offset + sizeof(uint8_t) * definitions::max_report_station_name_length;
	const uint8_t temperature_offset = report_peroid_offset + sizeof(uint8_t);
	const uint8_t crc_offset = temperature_offset + sizeof(double);

	uint8_t payload[100];

public:
	virtual bool SetPayload(uint8_t* payload, unsigned int length) override;
	virtual double GetValue() override;
	virtual void GetName(String& name) override;
	virtual uint8_t GetId() override;
	virtual uint8_t GetReportPeroid() override;
};

} // namespace mqtt_topic