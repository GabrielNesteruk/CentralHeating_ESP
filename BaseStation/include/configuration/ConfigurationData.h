#pragma once

#include <stdint.h>

namespace configuration
{
struct BaseStationConfigData
{
	uint8_t ssid[32];
	uint8_t password[64];
	uint8_t ip[16];
	uint8_t gateway[16];
	uint8_t mask[16];
	uint16_t crc;
};

} // namespace configuration