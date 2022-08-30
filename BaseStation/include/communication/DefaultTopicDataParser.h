#pragma once

#include "ITopicData.h"
#include "configuration/ConfigurationData.h"

namespace mqtt_topic
{

class DefaultTopicDataParser : public ITopicData
{
private:
	uint8_t payload[100];
	char name[configuration::ReportStationNameLength];

public:
	DefaultTopicDataParser();
	virtual void SetPayload(uint8_t* payload, unsigned int length) override;
	virtual double GetTemperature() override;
	virtual const char* GetName() override;
	virtual uint8_t GetId() override;
	virtual uint8_t GetReportPeroid() override;
};

} // namespace mqtt_topic