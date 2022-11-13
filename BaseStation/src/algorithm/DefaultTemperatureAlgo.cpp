#include "algorithm/DefaultTemperatureAlgo.h"

using namespace algorithm;

DefaultTemperatureAlgo::DefaultTemperatureAlgo(misc::AppState& appState, double hysteresis)
	: hysteresis{hysteresis}
	, appState{appState}
{ }

bool DefaultTemperatureAlgo::CompareSetpointWithValues(double setpoint,
													   double* reported_values,
													   size_t size)
{
	if(reported_values != nullptr)
	{
		double avg, sum, calculation = 0.0;
		for(size_t i{}; i < size; i++)
		{
			sum += reported_values[i];
		}
		avg = sum / static_cast<double>(size);

		if(appState.GetState() == misc::AppStates::INIT)
		{
			calculation = setpoint;
		}
		else if(appState.GetState() == misc::AppStates::HEATING)
		{
			calculation = setpoint - this->hysteresis;
		}
		else if(appState.GetState() == misc::AppStates::COOLING)
		{
			calculation = setpoint + this->hysteresis;
		}

		// 23.5 - set

		// 23.7 - heating
		// if 23.7 >= 23.5 + 0.2 -> start cooling

		// 23.3 - cooling
		// if 23.3 <= 23.5 - 0.2 -> start heating

		if(avg > calculation)
		{
			return false; // stop heating
		}
		else
		{
			return true; // keep heating
		}
	}
	else
	{
		return false;
	}
}