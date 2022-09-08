#pragma once

#include "IAlgorithm.h"

namespace algorithm
{

class DefaultTemperatureAlgo : public IAlgorithm<double>
{
private:
	double hysteresis;

public:
	DefaultTemperatureAlgo(double hysteresis = 0.0);

	virtual bool
	CompareSetpointWithValues(double setpoint, double* reported_values, size_t size) override;
};

} // namespace algorithm