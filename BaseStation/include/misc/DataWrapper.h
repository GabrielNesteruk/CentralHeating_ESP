#pragma once

#include "configuration/ConfigurationManager.h"
#include "misc/MemoryMap.h"
#include <definitions.h>

namespace data
{
class DataWrapper
{
private:
	double avg_temperature;
	double set_temperature;
	String ask_sensor_api;
	configuration::ConfigurationManager& config_manager;

public:
	DataWrapper(configuration::ConfigurationManager& config_manager)
		: avg_temperature{0.0}
		, config_manager{config_manager}
	{
		delay(1000);
		uint8_t tmp_buff[definitions::ask_sensors_api_key_length + 1];
		memset(tmp_buff, 0xFF, definitions::ask_sensors_api_key_length);
		auto config = config_manager.Get();
		this->set_temperature = config.setpoint;
		if(memcmp(config.api_key, tmp_buff, definitions::ask_sensors_api_key_length) != 0)
		{
			Serial.println("API key found in the config!");
			memcpy(tmp_buff, config.api_key, definitions::ask_sensors_api_key_length);
			tmp_buff[definitions::ask_sensors_api_key_length] = '\0';
			this->ask_sensor_api = (char*)tmp_buff;
			Serial.println("API:" + this->ask_sensor_api);
		}
		else
		{
			this->ask_sensor_api = "";
		}
	}

	double& getAvgTemperature()
	{
		return this->avg_temperature;
	}

	double& getSetTemperature()
	{
		return this->set_temperature;
	}

	String& getAskSensorApi()
	{
		return this->ask_sensor_api;
	}
};

} // namespace data
