#pragma once

#include <definitions.h>

namespace data
{
class DataWrapper
{
private:
	double current_temperature;
	double humidity;
	String ask_sensor_api;

public:
	DataWrapper()
		: current_temperature{0.0}
		, humidity{0.0}
	{ }

	double& getCurrentTemperature()
	{
		return this->current_temperature;
	}

	double& getHumidity()
	{
		return this->humidity;
	}

	String& getAskSensorApi()
	{
		return this->ask_sensor_api;
	}
};

} // namespace data
