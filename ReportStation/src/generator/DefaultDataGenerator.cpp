#include "generator/DefaultDataGenerator.h"
#include <CRC.h>
#include <string.h>

using namespace data_generator;

DefaultDataGenerator::DefaultDataGenerator(configuration::ConfigurationManager& config_manager)
	: config_manager(config_manager)
{ }

uint16_t DefaultDataGenerator::GenerateData(uint8_t* buffer, size_t buffer_length)
{
	uint16_t m_Length{}, data_size{};
	auto config = this->config_manager.Get();
	data_size = sizeof(config.uid) + sizeof(config.name) + sizeof(config.report_peroid) +
				sizeof(double) * 2 + sizeof(uint16_t);

	if(data_size > buffer_length)
	{
		return 0;
	}

	if(this->aht.begin())
	{
		double temperature, humidity;
		aht.getEvent(&this->humidity, &this->temp);
		temperature = this->temp.temperature;
		humidity = this->humidity.relative_humidity;

		memcpy(buffer + m_Length, &config.uid, sizeof(config.uid));
		m_Length += sizeof(config.uid);
		memcpy(buffer + m_Length, config.name, sizeof(config.name));
		m_Length += sizeof(config.name);
		memcpy(buffer + m_Length, &config.report_peroid, sizeof(config.report_peroid));
		m_Length += sizeof(config.report_peroid);
		memcpy(buffer + m_Length, &temperature, sizeof(double));
		m_Length += sizeof(double);
		memcpy(buffer + m_Length, &humidity, sizeof(double));
		m_Length += sizeof(double);

		uint16_t crc = crc16_CCITT(buffer, m_Length);
		memcpy(buffer + m_Length, &crc, sizeof(uint16_t));
		m_Length += sizeof(crc);
	}

	return m_Length;
}