#pragma once

#include <stdint.h>
#include <stddef.h>

namespace configuration
{
    constexpr size_t ReportStationNameLength = 20;
    constexpr size_t MaxRaportStations = 2;

    struct BaseStationConfiguration
    {
        uint8_t ssid[32];
        uint8_t password[64];
        uint8_t ip[16];
        uint8_t gateway[16];
        uint8_t mask[16];
        uint16_t crc;
    };

    struct ReportStationConfiguration
    {
        uint8_t id;
        char name[ReportStationNameLength];
        uint8_t ip[16];
        uint16_t crc;
    };

    struct ConfigurationData
    {
        BaseStationConfiguration base_station_config;
        ReportStationConfiguration report_stations_config[MaxRaportStations];
    };

}