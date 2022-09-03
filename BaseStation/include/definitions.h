#pragma once

#include <stdint.h>

namespace definitions
{

constexpr uint8_t max_report_stations{5};
constexpr uint8_t max_report_station_name_length{20};

constexpr const char* basic_station_name{"Basic Station"};
constexpr const char* broker_address{"broker.hivemq.com"};
constexpr const int broker_port{1883};
constexpr const char* topic_name{"centralHeating/roomTemperature"};
} // namespace definitions