#include "algorithm/DefaultTemperatureAlgo.h"
#include "definitions.h"
#include "misc/AppState.h"
#include <Arduino.h>

misc::AppState appState;
algorithm::DefaultTemperatureAlgo default_temp_measure_algorithm(appState,
																 definitions::default_hysteresis);

double f_t(int t)
{
	return (0.1 * t + 18.90);
}

double g_t(int t)
{
	return ((-0.05) * t + 23.25);
}

void setup()
{
	Serial.begin(115200);
	Serial.println();
}

double temp[] = {0.0};
int t = 1;
int total_time = 1;

uint8_t phase = 0;

void loop()
{
	if(phase != 3)
	{
		Serial.print(total_time, DEC);
		Serial.print(";");
	}

	if(phase == 0)
	{
		if(temp[0] < 23.00)
		{
			// ciagle zwiekszaj temp
			temp[0] = f_t(t);
			bool result = default_temp_measure_algorithm.CompareSetpointWithValues(22.5, temp, 1);
			Serial.print(temp[0], 2);
			Serial.print(";");
			Serial.print(result, DEC);
			Serial.println();

			t++;
		}
		else
		{
			phase = 1;
			t = 1;
		}
	}

	if(phase == 1)
	{
		// ochladzaj az temperatura bedzie wynosic 22.10
		if(temp[0] > 22.10)
		{
			temp[0] = g_t(t);
			bool result = default_temp_measure_algorithm.CompareSetpointWithValues(22.5, temp, 1);
			Serial.print(temp[0], 2);
			Serial.print(";");
			Serial.print(result, DEC);
			Serial.println();

			t++;
		}
		else
		{
			phase = 2;
			t = 33;
		}
	}

	if(phase == 2)
	{
		if(temp[0] < 23.00)
		{
			// ciagle zwiekszaj temp
			temp[0] = f_t(t);
			bool result = default_temp_measure_algorithm.CompareSetpointWithValues(22.5, temp, 1);
			Serial.print(temp[0], 2);
			Serial.print(";");
			Serial.print(result, DEC);
			Serial.println();

			t++;
		}
		else
		{
			phase = 3;
			t = 1;
		}
	}

	if(phase == 3)
	{
		while(true)
		{
			delay(100);
		}
	}

	total_time++;
}