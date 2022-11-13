#pragma once

#include "IAlgorithm.h"
#include "misc/AppState.h"

namespace algorithm
{

class DefaultTemperatureAlgo : public IAlgorithm<double>
{
private:
	double hysteresis;
	misc::AppState& appState;

public:
	DefaultTemperatureAlgo(misc::AppState& appState, double hysteresis = 0.0);

	virtual bool
	CompareSetpointWithValues(double setpoint, double* reported_values, size_t size) override;
};

} // namespace algorithm