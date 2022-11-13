#include "controller/WorkFlowController.h"

using namespace controller;

WorkFlowController::WorkFlowController(configuration::ConfigurationManager& config_manager,
									   communication::WiFiAggregator& wifi_aggregator,
									   data_generator::IDataGenerator* data_generator)
	: config_manager(config_manager)
	, wifi_aggregator(wifi_aggregator)
	, data_generator(data_generator)
	, last_latched_time(0)
{ }

void WorkFlowController::Service()
{
	auto config = config_manager.Get();
	auto time = millis();
	if(time - this->last_latched_time >=
	   static_cast<unsigned long>(config.report_peroid * 1000 * 60))
	{
		this->last_latched_time = time;
		uint16_t data_length =
			this->data_generator->GenerateData(this->data_buffer, sizeof(this->data_buffer));
		this->wifi_aggregator.SendData(this->data_buffer, data_length);

		// indicate that data were sent
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
		digitalWrite(LED_BUILTIN, HIGH);

		Serial.println("Data sent!");
		for(unsigned int i = 0; i < data_length; i++)
		{
			char buff[3];
			sprintf(buff, "%02X", this->data_buffer[i]);
			Serial.print(buff);
		}
		Serial.println("");
	}
}