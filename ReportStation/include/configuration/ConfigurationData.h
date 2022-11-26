#pragma once

#include "definitions.h"
#include <stdint.h>

namespace configuration
{
struct ReportStationConfigData
{
	uint8_t ssid[32 + 1];
	uint8_t password[64 + 1];
	uint8_t ip[16 + 1];
	uint8_t gateway[16 + 1];
	uint8_t mask[16 + 1];
	uint8_t uid;
	uint8_t name[definitions::max_report_station_name_length + 1];
	uint8_t report_peroid;
	uint8_t api_key[definitions::ask_sensors_api_key_length + 1];
};

} // namespace configuration