#include "Constants.h"
#include "algorithm/DefaultTemperatureAlgo.h"
#include "communication/DefaultTopicDataParser.h"
#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"
#include "controller/CentralHeatingWorkController.h"
#include "device/AHT20.h"
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
	device::AHT20 temperature_sensors[constants::max_report_stations];
	algorithm::DefaultTemperatureAlgo default_temp_measure_algorithm(
		definitions::default_hysteresis);

	configuration::ConfigurationManager config_manager;
	controller::CentralHeatingWorkController central_heating_controller{
		temperature_sensors,
		&default_temp_measure_algorithm,
		definitions::default_setpoint,
		data_storage};
	mqtt_topic::DefaultTopicDataParser default_topic_data_parser;
	communication::WiFiAggregator wifi_aggregator(WiFi,
												  config_manager,
												  &default_topic_data_parser,
												  &central_heating_controller,
												  data_storage);

	wifi_aggregator.Init();

	while(true)
	{
		wifi_aggregator.Service();
	}
}