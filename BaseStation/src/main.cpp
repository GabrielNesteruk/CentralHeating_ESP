#include "communication/DefaultTopicDataParser.h"
#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"
#include <Arduino.h>


configuration::ConfigurationManager config_manager;
mqtt_topic::DefaultTopicDataParser default_topic_data_parser;
communication::WiFiAggregator wifi_aggregator(WiFi, config_manager, &default_topic_data_parser);

void setup()
{
	Serial.begin(115200);
	Serial.println();
	wifi_aggregator.Init();
}

void loop() { }