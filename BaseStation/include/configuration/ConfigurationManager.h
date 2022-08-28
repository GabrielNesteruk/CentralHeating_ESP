#pragma once

#include "ConfigurationData.h"
#include "IPAddress.h"
#include <stdint.h>

namespace configuration
{

class ConfigurationManager
{
private:
	const uint32_t config_base_address{0};
	ConfigurationData data;
	uint8_t current_report_stations;

public:
	/**
         * @brief Saves configuration data for base station and report stations
         *
         * @return true if save was successful
         * @return false if there was flash error
         */
	bool Save();
	/**
         * @brief Loads configuration data for base station and report stations from flash memory
         *
         * @return true if there is base station config
         * @return false if there isn't base station config
         */
	bool Load();

	/**
     * @brief Clears configuration data
     * 
     */
	void Clear();

	/**
         * @brief Adds new report station
         *
         * @param data report station config
         * @return if report station was added
         */
	bool AddReportStationConfig(const ReportStationConfiguration& data);

	/**
         * @brief Updates existing report station
         *
         * @param data report station config
         * @return if report station was updated
         */
	bool UpdateReportStationConfig(const ReportStationConfiguration& data);

	/**
         * @brief Set the new base station configuration
         *
         * @param data base station config
         */
	void SetBaseStationConfig(const BaseStationConfiguration& data);

	/**
     * @brief Set the Base Station Config ip address
     * 
     * @param ip IpAddress object
     */
	void SetBaseStationConfigIp(const IPAddress& ip);

	/**
    * @brief Get the maximum raport stations
    *
    * @return size_t maximum raport stations
    */
	size_t GetMaxRaportStations();

	/**
     * @brief Get the Current Base Station Config object
     * 
     * @return const BaseStationConfiguration& 
     */
	const BaseStationConfiguration& GetCurrentBaseStationConfig();
};

} // namespace configuration