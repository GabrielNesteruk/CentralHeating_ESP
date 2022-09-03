#pragma once

#include "WorkFlowController.h"

namespace controller
{
class CentralHeatingWorkController : public WorkFlowController<double>
{
protected:
	virtual void Service() override { }

public:
	virtual void UpdateReportStation(mqtt_topic::ITopicData<double>* topic_data) override { }
};
} // namespace controller