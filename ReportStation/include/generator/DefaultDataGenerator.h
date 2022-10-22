#pragma once

#include <Adafruit_AHTX0.h>
#include <IDataGenerator.h>
#include <configuration/ConfigurationManager.h>
#include <misc/DataWrapper.h>

namespace data_generator
{
class DefaultDataGenerator : public IDataGenerator
{
private:
	configuration::ConfigurationManager& config_manager;
	Adafruit_AHTX0 aht;
	sensors_event_t temp, humidity;
	data::DataWrapper& data_storage;

public:
	DefaultDataGenerator(configuration::ConfigurationManager& config_manager,
						 data::DataWrapper& data_storage);
	virtual uint16_t GenerateData(uint8_t* buffer, size_t buffer_length) override;
};

} // namespace data_generator