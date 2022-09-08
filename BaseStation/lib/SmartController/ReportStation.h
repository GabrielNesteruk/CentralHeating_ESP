#pragma once

#include <Arduino.h>
#include <ESP8266WiFi.h>

namespace device
{

template <typename T>
class ReportStation
{
protected:
	uint8_t uid;
	IPAddress ip;
	String name;
	uint8_t report_peroid;
	T last_value;

public:
	ReportStation()
	{
		uid = UINT8_MAX;
		ip = static_cast<uint32_t>(0);
		name = "";
		report_peroid = 0;
	}

	ReportStation(const uint8_t& uid,
				  const IPAddress& ip,
				  const String& name,
				  const uint8_t& report_peroid)
	{
		this->uid = uid;
		this->ip = ip;
		this->name = name;
		this->report_peroid = report_peroid;
	}

	const uint8_t& GetUID() const
	{
		return uid;
	}
	void SetUID(const uint8_t& uid)
	{
		this->uid = uid;
	}
	const IPAddress& GetIp() const
	{
		return this->ip;
	}
	void SetIp(const IPAddress& ip)
	{
		this->ip = ip;
	}
	String& GetName()
	{
		return this->name;
	}
	void SetName(const String& name)
	{
		this->name = name;
	}
	const uint8_t& GetReportPeroid() const
	{
		return this->report_peroid;
	}
	void SetReportPeroid(const uint8_t& report_peroid)
	{
		this->report_peroid = report_peroid;
	}

	const virtual T GetLastReportedValue() const = 0;
	virtual void SetLastReportedValue(T value) = 0;
};

} // namespace device