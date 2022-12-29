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
		double avg = 0.0, sum = 0.0, calculation = 0.0;
		for(size_t i{}; i < size; i++)
		{
			sum += reported_values[i];
		}
		avg = sum / static_cast<double>(size);

		if(appState.GetState() == misc::AppStates::INIT)
		{
			// check in what state is app
			if(avg >= setpoint - hysteresis)
				appState.SetState(misc::AppStates::STANDBY);
			else
				appState.SetState(misc::AppStates::HEATING);
		}

		if(appState.GetState() == misc::AppStates::HEATING)
		{
			calculation = setpoint;
			if(avg >= calculation)
			{
				// temperature exceeded setpoint so stop heating and go into standby
				appState.SetState(misc::AppStates::STANDBY);
				return false; // stop heating
			}
			else
			{
				// else just keep heating
				return true; // start heating
			}
		}
		else if(appState.GetState() == misc::AppStates::STANDBY)
		{
			calculation = setpoint - this->hysteresis;
			if(avg < calculation)
			{
				// temperature is below setpoint so start heating
				appState.SetState(misc::AppStates::HEATING);
				return true;
			}
			else
			{
				// else do nothing, just wait
				return false;
			}
		}

		// Serial.println("Setpoint: ");
		// Serial.print(setpoint, DEC);
		// Serial.println();

		// Serial.println("Avg: ");
		// Serial.print(avg, 2);
		// Serial.println();

		// just for safety reasons...
		return false;
	}
	else
	{
		// array of temperatures is a nullptr
		return false;
	}
}