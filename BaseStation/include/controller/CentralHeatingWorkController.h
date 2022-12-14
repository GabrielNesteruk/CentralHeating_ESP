#pragma once

#include "ReportStation.h"
#include "WorkFlowController.h"
#include "device/AHT20.h"
#include "device/Lcd.h"
#include "device/Relay.h"
#include "misc/AppState.h"
#include "misc/DataWrapper.h"
#include <IAlgorithm.h>

namespace controller
{
class CentralHeatingWorkController : public WorkFlowController<double>
{

private:
	double setpoint;
	algorithm::IAlgorithm<double>* temperature_algorithm;
	data::DataWrapper& data_storage;
	uint8_t last_saved_uid;
	lcd::Lcd& lcd;
	device::Relay& relay;
	misc::AppState& appState;

	device::ReportStation<double>* getReportStationByUID(uint8_t uid);

protected:
	virtual void Service() override;

public:
	CentralHeatingWorkController(device::ReportStation<double>* report_stations_array,
								 algorithm::IAlgorithm<double>* temperature_algorithm,
								 double default_setpoint,
								 data::DataWrapper& data_storage,
								 lcd::Lcd& lcd,
								 device::Relay& relay,
								 misc::AppState& appState);
	virtual void UpdateReportStation(mqtt_topic::ITopicData<double>* topic_data) override;
	virtual void UpdateInternalValues() override;
};
} // namespace controller