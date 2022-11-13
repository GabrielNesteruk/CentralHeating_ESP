#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"
#include "controller/WorkFlowController.h"
#include "device/PushButton.h"
#include "generator/DefaultDataGenerator.h"
#include "misc/DataWrapper.h"
#include <Arduino.h>

volatile byte state = HIGH;

void IRAM_ATTR change()
{
	state = !state;
	digitalWrite(LED_BUILTIN, state);
}

void setup()
{
	Serial.begin(115200);
	Serial.println();
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
	data::DataWrapper data_storage;
	configuration::ConfigurationManager config_manager;
	device::PushButton pushButton(config_manager);
	data_generator::DefaultDataGenerator default_data_generator{config_manager, data_storage};
	communication::WiFiAggregator wifi_aggregator(WiFi, config_manager, data_storage, pushButton);
	controller::WorkFlowController controller{
		config_manager, wifi_aggregator, &default_data_generator};

	wifi_aggregator.Init();

	while(true)
	{
		wifi_aggregator.Service();
		controller.Service();
		pushButton.Service();
	}
}