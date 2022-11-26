#pragma once

#include "ConfigurationData.h"
#include "IPAddress.h"
#include <IConfiguration.h>
#include <stdint.h>

namespace configuration
{

class ConfigurationManager : public IConfiguration<ReportStationConfigData>
{
private:
	struct ConfigurationData
	{
		ReportStationConfigData data;
		uint16_t crc;
	};

	ConfigurationData config_data;
	const uint32_t config_base_address{0};

public:
	ConfigurationManager();
	virtual bool Save() const override;
	virtual bool Load() override;
	virtual void Clear() const override;
	virtual void Update(const ReportStationConfigData& data) override;
	virtual ReportStationConfigData Get() const override;
};

} // namespace configuration