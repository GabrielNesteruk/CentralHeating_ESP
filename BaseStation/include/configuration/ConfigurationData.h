#pragma once

#include "definitions.h"
#include <stdint.h>

namespace configuration
{
#pragma pack(push, 1)
struct BaseStationConfigData
{
	uint8_t ssid[32 + 1];
	uint8_t password[64 + 1];
	uint8_t ip[16 + 1];
	uint8_t gateway[16 + 1];
	uint8_t mask[16 + 1];
	uint8_t api_key[definitions::ask_sensors_api_key_length + 1];
	double setpoint;
};
#pragma pack(pop)

} // namespace configuration