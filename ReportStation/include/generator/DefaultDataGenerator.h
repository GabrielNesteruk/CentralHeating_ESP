#pragma once

#include <Adafruit_AHTX0.h>
#include <IDataGenerator.h>
#include <configuration/ConfigurationManager.h>

namespace data_generator
{
class DefaultDataGenerator : public IDataGenerator
{
private:
	configuration::ConfigurationManager& config_manager;
	Adafruit_AHTX0 aht;
	sensors_event_t temp, humidity;

public:
	DefaultDataGenerator(configuration::ConfigurationManager& config_manager);
	virtual uint16_t GenerateData(uint8_t* buffer, size_t buffer_length) override;
};

} // namespace data_generator