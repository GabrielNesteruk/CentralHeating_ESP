#pragma once

#include "ReportStation.h"

namespace device
{
template <typename T>
class AHT20 : public ReportStation<T>
{
private:
	T last_temperature;

public:
	AHT20()
		: ReportStation()
	{
		last_temperature = 0.0;
	}

	AHT20(const uint8_t& uid, const IPAddress& ip, const String& name, const uint8_t& report_peroid)
		: ReportStation(uid, ip, name, report_peroid)
		, this()
	{ }

	virtual T GetLastReportedValue() const override
	{
		return last_temperature;
	}

	virtual void SetLastReportedValue(T value) override
	{
		this->last_temperature = value;
	}
};

} // namespace device