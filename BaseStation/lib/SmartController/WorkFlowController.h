#pragma once

#include "Constants.h"
#include "ITopicData.h"
#include "ReportStation.h"


namespace controller
{
template <typename T>
class WorkFlowController
{
protected:
	device::ReportStation<T>* report_stations[constants::max_report_stations];
	uint8_t active_report_stations{0};

	virtual void Service() = 0;

public:
	virtual void UpdateReportStation(mqtt_topic::ITopicData<T>* topic_data) = 0;
};

} // namespace controller