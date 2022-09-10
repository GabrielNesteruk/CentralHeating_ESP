#pragma once

#include "definitions.h"
#include <stdint.h>


namespace configuration
{
struct ReportStationConfigData
{
	uint8_t ssid[32];
	uint8_t password[64];
	uint8_t ip[16];
	uint8_t gateway[16];
	uint8_t mask[16];
	uint8_t name[definitions::max_report_station_name_length];
	uint8_t report_peroid;
};

} // namespace configuration