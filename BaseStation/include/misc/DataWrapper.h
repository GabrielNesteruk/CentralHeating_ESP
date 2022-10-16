#pragma once

#include <definitions.h>

namespace data
{
class DataWrapper
{
private:
	double avg_temperature;
	double set_temperature;

public:
	DataWrapper()
		: avg_temperature{0.0}
		, set_temperature{definitions::default_setpoint}
	{ }

	double& getAvgTemperature()
	{
		return this->avg_temperature;
	}

	double& getSetTemperature()
	{
		return this->set_temperature;
	}
};

} // namespace data
