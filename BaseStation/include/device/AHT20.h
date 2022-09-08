#pragma once

#include "ReportStation.h"

namespace device
{
class AHT20 : public ReportStation<double>
{

public:
	AHT20()
	{
		memset(this->last_value, 0x00, sizeof(this->last_value));
	}

	AHT20(const uint8_t& uid, const IPAddress& ip, const String& name, const uint8_t& report_peroid)
		: ReportStation(uid, ip, name, report_peroid)
	{
		memset(this->last_value, 0x00, sizeof(this->last_value));
	}

	virtual const double GetLastReportedValue(size_t index) const override
	{
		if((index < 2) && (index < GetMaxReportStationValues()))
		{
			return this->last_value[index];
		}
		else
		{
			return 0.0;
		}
	}

	virtual void SetLastReportedValue(double value, size_t index) override
	{
		if((index < 2) && (index < GetMaxReportStationValues()))
		{
			this->last_value[index] = value;
		}
	}
};

} // namespace device