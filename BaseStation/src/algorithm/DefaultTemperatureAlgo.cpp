#include "algorithm/DefaultTemperatureAlgo.h"

using namespace algorithm;

DefaultTemperatureAlgo::DefaultTemperatureAlgo(double hysteresis)
	: hysteresis{hysteresis}
{ }

bool DefaultTemperatureAlgo::CompareSetpointWithValues(double setpoint,
													   double* reported_values,
													   size_t size)
{
	if(reported_values != nullptr)
	{
		double avg, sum = 0.0;
		for(size_t i{}; i < size; i++)
		{
			sum += reported_values[i];
		}
		avg = sum / static_cast<double>(size);

		Serial.println("AVG:");
		Serial.print(avg, 2);
		Serial.println("");

		if(avg > setpoint - this->hysteresis)
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