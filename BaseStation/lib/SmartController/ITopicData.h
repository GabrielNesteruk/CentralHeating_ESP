#pragma once

#include <stdint.h>

namespace mqtt_topic
{
template <typename T>
class ITopicData
{
public:
	virtual void SetPayload(uint8_t* payload, unsigned int length) = 0;
	virtual T GetValue() = 0;
	virtual const char* GetName() = 0;
	virtual uint8_t GetId() = 0;
	virtual uint8_t GetReportPeroid() = 0;
};

} // namespace mqtt_topic