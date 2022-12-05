#include "configuration/ConfigurationManager.h"
#include <CRC.h>
#include <EEPROM.h>
#include <string.h>

using namespace configuration;

ConfigurationManager::ConfigurationManager()
{
	memset(&(this->config_data), 0x00, sizeof(this->config_data));
	Load();
}

bool ConfigurationManager::Save() const
{
	bool result;

	EEPROM.begin(sizeof(this->config_data));
	EEPROM.put(this->config_base_address, this->config_data);
	result = EEPROM.commit();
	EEPROM.end();

	return result;
}

bool ConfigurationManager::Load()
{
	EEPROM.begin(sizeof(this->config_data));
	EEPROM.get(config_base_address, this->config_data);
	EEPROM.end();

	// check for base station data
	uint16_t crc =
		crc16_CCITT(reinterpret_cast<uint8_t*>(&this->config_data), sizeof(this->config_data) - 2);

	if(crc == this->config_data.crc)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ConfigurationManager::Clear() const
{
	EEPROM.begin(sizeof(this->config_data));
	for(size_t i{this->config_base_address}; i < sizeof(this->config_data); i++)
	{
		EEPROM.write(i, 0xFF);
	}
	EEPROM.commit();
	EEPROM.end();
}

void ConfigurationManager::Update(const BaseStationConfigData& data)
{
	memcpy(&this->config_data.data, &data, sizeof(BaseStationConfigData));
	this->config_data.crc =
		crc16_CCITT(reinterpret_cast<uint8_t*>(&this->config_data), sizeof(this->config_data) - 2);
	this->Save();
}

BaseStationConfigData ConfigurationManager::Get() const
{
	return this->config_data.data;
}
