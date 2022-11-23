#pragma once

#include "ConfigurationData.h"
#include "IConfiguration.h"
#include "IPAddress.h"
#include "misc/MemoryMap.h"
#include <stdint.h>

namespace configuration
{

class ConfigurationManager : public IConfiguration<BaseStationConfigData>
{
private:
	struct ConfigurationData
	{
		BaseStationConfigData data;
		uint16_t crc;
	};

	ConfigurationData config_data;
	const uint32_t config_base_address{memory::configuration_data_base_address};

public:
	ConfigurationManager();
	virtual bool Save() const override;
	virtual bool Load() override;
	virtual void Clear() const override;
	virtual void Update(const BaseStationConfigData& data) override;
	virtual BaseStationConfigData Get() const override;
};

} // namespace configuration