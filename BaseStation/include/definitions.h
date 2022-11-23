#pragma once

#include <stdint.h>

namespace definitions
{

constexpr uint8_t max_report_stations{5};
constexpr uint8_t max_report_station_name_length{20};
constexpr double default_hysteresis{0.2};
constexpr double default_setpoint{15.0};
constexpr double default_humidity{0.0};

constexpr const char* basic_station_name{"Basic Station"};
constexpr const char* broker_username{"centralheating"};
constexpr const char* broker_password{"ineedwarmhouse"};
constexpr const char* broker_address{"f2c11b1d3819403e93d49f7036dddfdf.s1.eu.hivemq.cloud"};
constexpr const int broker_port{8883};
constexpr const char* topic_name{"centralHeating/roomTemperature"};
constexpr const char* topic_ip_broadcast{"centralHeating/baseStationIp"};

constexpr uint8_t mqtt_broadcast_waiting_time{20};

constexpr const char* ask_sensors_host{"api.asksensors.com"};
constexpr uint8_t ask_sensors_api_key_length{32};

constexpr uint8_t LED_BUILTIN{2};

enum ERROR_TYPE
{
	WIFI_CONNECTION_ERROR = 0x00,
	MQTT_CONNECTION_ERROR = 0x01,
	REPORT_STATION_FULL_LIST_ERROR = 0x03
};

} // namespace definitions