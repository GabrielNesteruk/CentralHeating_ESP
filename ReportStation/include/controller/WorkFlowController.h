#pragma once
#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"
#include "misc/DataWrapper.h"
#include <IDataGenerator.h>

namespace controller
{

class WorkFlowController
{
private:
	configuration::ConfigurationManager& config_manager;
	communication::WiFiAggregator& wifi_aggregator;
	data_generator::IDataGenerator* data_generator;
	data::DataWrapper& data_storage;
	unsigned long last_latched_time;
	uint8_t data_buffer[100];

	void SendTemperatureToCloud();

public:
	WorkFlowController(configuration::ConfigurationManager& config_manager,
					   communication::WiFiAggregator& wifi_aggregator,
					   data_generator::IDataGenerator* data_generator,
					   data::DataWrapper& data_storage);

	void Service();
};

} // namespace controller