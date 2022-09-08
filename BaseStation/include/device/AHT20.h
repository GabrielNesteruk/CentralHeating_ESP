#pragma once

#include "ReportStation.h"

namespace device
{
class AHT20 : public ReportStation<double>
{

public:
	AHT20()
	{
		this->last_value = 0.0;
	}

	AHT20(const uint8_t& uid, const IPAddress& ip, const String& name, const uint8_t& report_peroid)
		: ReportStation(uid, ip, name, report_peroid)
	{
		this->last_value = 0.0;
	}

	virtual const double GetLastReportedValue() const override
	{
		return this->last_value;
	}

	virtual void SetLastReportedValue(double value) override
	{
		this->last_value = value;
	}
};

} // namespace device