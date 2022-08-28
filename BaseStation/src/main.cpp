#include <Arduino.h>
#include "communication/WiFiAggregator.h"
#include "configuration/ConfigurationManager.h"

configuration::ConfigurationManager config_manager;
communication::WiFiAggregator wifi_aggregator(WiFi, config_manager);

void setup()
{
  Serial.begin(115200);
  Serial.println();
  wifi_aggregator.Init();
}

void loop()
{
}