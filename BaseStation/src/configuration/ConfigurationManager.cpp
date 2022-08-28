#include "configuration/ConfigurationManager.h"
#include <CRC.h>
#include <EEPROM.h>
#include <string.h>

using namespace configuration;

bool ConfigurationManager::Load()
{
	EEPROM.begin(sizeof(this->data));
	EEPROM.get(config_base_address, this->data);
	EEPROM.end();

	// check for raport stations
	for(size_t i{}; i < MaxRaportStations; i++)
	{
		uint16_t crc = crc16_CCITT(reinterpret_cast<uint8_t*>(&this->data.report_stations_config),
								   sizeof(this->data.report_stations_config) - 2);

		if(crc == this->data.report_stations_config[i].crc)
		{
			this->current_report_stations++;
		}
	}

	// check for base station data
	uint16_t crc = crc16_CCITT(reinterpret_cast<uint8_t*>(&this->data.base_station_config),
							   sizeof(this->data.base_station_config) - 2);

	if(crc == this->data.base_station_config.crc)
	{
		// base config data is okay
		return true;
	}
	else
	{
		return false;
	}
}

bool ConfigurationManager::Save()
{
	bool result;

	EEPROM.begin(sizeof(this->data));
	EEPROM.put(this->config_base_address, this->data);
	result = EEPROM.commit();
	EEPROM.end();

	return result;
}

void ConfigurationManager::Clear()
{
	EEPROM.begin(sizeof(this->data));
	for(size_t i{this->config_base_address}; i < sizeof(this->data); i++)
	{
		EEPROM.write(i, 0xFF);
	}
	EEPROM.commit();
	EEPROM.end();
}

bool ConfigurationManager::AddReportStationConfig(const ReportStationConfiguration& data)
{
	if(data.id < MaxRaportStations && data.id > this->current_report_stations)
	{
		memcpy(
			&this->data.report_stations_config[data.id], &data, sizeof(ReportStationConfiguration));
		this->current_report_stations++;
		return true;
	}
	else
	{
		return false;
	}
}

bool ConfigurationManager::UpdateReportStationConfig(const ReportStationConfiguration& data)
{
	if(data.id < MaxRaportStations && data.id < this->current_report_stations)
	{
		memcpy(
			&this->data.report_stations_config[data.id], &data, sizeof(ReportStationConfiguration));
		return true;
	}
	else
	{
		return false;
	}
}

void ConfigurationManager::SetBaseStationConfig(const BaseStationConfiguration& data)
{
	memcpy(&this->data.base_station_config, &data, sizeof(BaseStationConfiguration));
	this->data.base_station_config.crc =
		crc16_CCITT(reinterpret_cast<uint8_t*>(&this->data.base_station_config),
					sizeof(this->data.base_station_config) - 2);
}

void ConfigurationManager::SetBaseStationConfigIp(const IPAddress& ip)
{
	ip.toString().toCharArray(reinterpret_cast<char*>(this->data.base_station_config.ip),
							  sizeof(this->data.base_station_config.ip));
	this->data.base_station_config.crc =
		crc16_CCITT(reinterpret_cast<uint8_t*>(&this->data.base_station_config),
					sizeof(this->data.base_station_config) - 2);
}

size_t ConfigurationManager::GetMaxRaportStations()
{
	return MaxRaportStations;
}

const BaseStationConfiguration& ConfigurationManager::GetCurrentBaseStationConfig()
{
	return this->data.base_station_config;
}