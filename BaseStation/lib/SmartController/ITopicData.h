#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace mqtt_topic
{
template <typename T>
class ITopicData
{
public:
	virtual bool SetPayload(uint8_t* payload, unsigned int length) = 0;
	/**
	 * @brief Get the value T, maybe there are multiple values in a frame so we have to distinguish them by index
	 * 
	 * @param index unique index of the value
	 * @return T value
	 */
	virtual T GetValue(size_t index) = 0;
	virtual void GetName(String& name) = 0;
	virtual uint8_t GetId() = 0;
	virtual uint8_t GetReportPeroid() = 0;
};

} // namespace mqtt_topic