#include "communication/DefaultTopicDataParser.h"
#include <CRC.h>

using namespace mqtt_topic;

bool DefaultTopicDataParser::SetPayload(uint8_t* payload, unsigned int length)
{
	if(length > sizeof(this->payload))
	{
		length = sizeof(this->payload);
	}

	memcpy(this->payload, payload, length);

	uint16_t crc = crc16_CCITT(this->payload, length - 2);
	uint16_t m_crc;
	memcpy(&m_crc, this->payload + this->crc_offset, sizeof(m_crc));
	if(crc == m_crc)
	{
		return true;
	}

	return false;
}

double DefaultTopicDataParser::GetValue()
{
	double m_val;
	memcpy(&m_val, this->payload + this->temperature_offset, sizeof(m_val));
	return m_val;
}

void DefaultTopicDataParser::GetName(String& name)
{
	char name_char_array[definitions::max_report_station_name_length];
	memcpy(name_char_array,
		   this->payload + this->name_offset,
		   sizeof(uint8_t) * definitions::max_report_station_name_length);
	name = name_char_array;
}

uint8_t DefaultTopicDataParser::GetId()
{
	uint8_t m_id;
	memcpy(&m_id, this->payload + this->id_offset, sizeof(m_id));
	return m_id;
}

uint8_t DefaultTopicDataParser::GetReportPeroid()
{
	uint8_t m_peroid;
	memcpy(&m_peroid, this->payload + this->report_peroid_offset, sizeof(m_peroid));
	return m_peroid;
}