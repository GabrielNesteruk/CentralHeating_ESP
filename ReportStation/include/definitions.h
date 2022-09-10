#pragma once

#include <stdint.h>

namespace definitions
{
constexpr uint8_t max_report_station_name_length{20};
constexpr const char* default_report_station_name{"Report Station"};
constexpr const char* broker_username{"centralheating"};
constexpr const char* broker_password{"ineedwarmhouse"};
constexpr const char* broker_address{"db98cc4b26c04454ba817c7d45c767e1.s1.eu.hivemq.cloud"};
constexpr const int broker_port{8883};
constexpr const char* topic_name{"centralHeating/roomTemperature"};
} // namespace definitions
