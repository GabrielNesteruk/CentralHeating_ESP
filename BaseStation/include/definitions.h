#pragma once

#include <stdint.h>

namespace definitions
{

constexpr uint8_t max_report_stations{5};
constexpr uint8_t max_report_station_name_length{20};
constexpr double default_hysteresis{0.0};
constexpr double default_setpoint{15.0};

constexpr const char* basic_station_name{"Basic Station"};
constexpr const char* broker_username{"centralheating"};
constexpr const char* broker_password{"ineedwarmhouse"};
constexpr const char* broker_address{"db98cc4b26c04454ba817c7d45c767e1.s1.eu.hivemq.cloud"};
constexpr const int broker_port{8883};
constexpr const char* topic_name{"centralHeating/roomTemperature"};
} // namespace definitions