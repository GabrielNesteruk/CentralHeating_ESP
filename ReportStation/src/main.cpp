#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"
#include "controller/WorkFlowController.h"
#include "generator/DefaultDataGenerator.h"
#include "misc/DataWrapper.h"
#include <Arduino.h>

void setup()
{
	Serial.begin(115200);
	Serial.println();
}

void loop()
{
	data::DataWrapper data_storage;
	configuration::ConfigurationManager config_manager;
	data_generator::DefaultDataGenerator default_data_generator{config_manager, data_storage};
	communication::WiFiAggregator wifi_aggregator(WiFi, config_manager, data_storage);
	controller::WorkFlowController controller{
		config_manager, wifi_aggregator, &default_data_generator};

	wifi_aggregator.Init();

	while(true)
	{
		wifi_aggregator.Service();
		controller.Service();
	}
}