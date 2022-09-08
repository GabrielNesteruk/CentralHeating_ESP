#pragma once

#include "ReportStation.h"
#include "WorkFlowController.h"
#include "device/AHT20.h"
#include <IAlgorithm.h>

namespace controller
{
class CentralHeatingWorkController : public WorkFlowController<double>
{

private:
	double setpoint;
	algorithm::IAlgorithm<double>* temperature_algorithm;

protected:
	virtual void Service() override;

public:
	CentralHeatingWorkController(device::ReportStation<double>* report_stations_array,
								 algorithm::IAlgorithm<double>* temperature_algorithm,
								 double default_setpoint);
	virtual void UpdateReportStation(mqtt_topic::ITopicData<double>* topic_data) override;
};
} // namespace controller