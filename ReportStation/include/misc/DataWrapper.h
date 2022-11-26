#pragma once

#include <definitions.h>
#include <string.h>

namespace data
{
class DataWrapper
{
private:
	double current_temperature;
	double humidity;
	String ask_sensor_api;
	configuration::ConfigurationManager& config_manager;

public:
	DataWrapper(configuration::ConfigurationManager& config_manager)
		: current_temperature{0.0}
		, humidity{0.0}
		, config_manager{config_manager}
	{
		delay(1000);
		uint8_t tmp_buff[definitions::ask_sensors_api_key_length + 1];
		memset(tmp_buff, 0x00, definitions::ask_sensors_api_key_length + 1);
		config_manager.Load();
		auto config = config_manager.Get();

		if(memcmp(tmp_buff, config.api_key, sizeof(config.api_key)) == 0)
		{
			this->ask_sensor_api = "";
		}
		else
		{
			Serial.println("API key found in the config!");
			memcpy(tmp_buff, config.api_key, definitions::ask_sensors_api_key_length);
			tmp_buff[definitions::ask_sensors_api_key_length] = '\0';
			this->ask_sensor_api = (char*)tmp_buff;
			Serial.println("API:" + this->ask_sensor_api);
		}
	}

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
