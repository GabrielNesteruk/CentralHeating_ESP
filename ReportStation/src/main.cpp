#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"
#include "controller/WorkFlowController.h"
#include "generator/DefaultDataGenerator.h"
#include <Arduino.h>

void setup()
{
	Serial.begin(115200);
	Serial.println();
}

void loop()
{
	configuration::ConfigurationManager config_manager;
	communication::WiFiAggregator wifi_aggregator(WiFi, config_manager);
	data_generator::DefaultDataGenerator default_data_generator{config_manager};
	controller::WorkFlowController controller{
		config_manager, wifi_aggregator, &default_data_generator};

	wifi_aggregator.Init();

	while(true)
	{
		wifi_aggregator.Service();
		controller.Service();
	}
}