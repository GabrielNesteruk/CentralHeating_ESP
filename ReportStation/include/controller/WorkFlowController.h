#pragma once
#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"
#include <IDataGenerator.h>

namespace controller
{

class WorkFlowController
{
private:
	configuration::ConfigurationManager& config_manager;
	communication::WiFiAggregator& wifi_aggregator;
	data_generator::IDataGenerator* data_generator;
	unsigned long last_latched_time;
	uint8_t data_buffer[100];

public:
	WorkFlowController(configuration::ConfigurationManager& config_manager,
					   communication::WiFiAggregator& wifi_aggregator,
					   data_generator::IDataGenerator* data_generator);

	void Service();
};

} // namespace controller