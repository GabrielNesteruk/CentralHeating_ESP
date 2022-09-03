#include "communication/DefaultTopicDataParser.h"
#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"
#include "controller/CentralHeatingWorkController.h"
#include <Arduino.h>

static configuration::ConfigurationManager config_manager;
static controller::CentralHeatingWorkController central_heating_controller;
static mqtt_topic::DefaultTopicDataParser default_topic_data_parser;
static communication::WiFiAggregator
	wifi_aggregator(WiFi, config_manager, &default_topic_data_parser, &central_heating_controller);

void setup()
{
	Serial.begin(115200);
	Serial.println();
	wifi_aggregator.Init();
}

void loop()
{
	wifi_aggregator.Service();
}