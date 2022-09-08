#include "controller/CentralHeatingWorkController.h"
#include "Constants.h"
#include "communication/DefaultTopicDataParser.h"

using namespace controller;

CentralHeatingWorkController::CentralHeatingWorkController(
	device::ReportStation<double>* report_stations_array,
	algorithm::IAlgorithm<double>* temperature_algorithm,
	double default_setpoint)
	: setpoint{default_setpoint}
	, temperature_algorithm{temperature_algorithm}
{
	for(size_t i{}; i < constants::max_report_stations; i++)
	{
		this->report_stations[i] = &report_stations_array[i];
	}
}

void CentralHeatingWorkController::UpdateReportStation(mqtt_topic::ITopicData<double>* topic_data)
{

	device::ReportStation<double>* report_station{nullptr};
	for(size_t i{}; i < constants::max_report_stations; i++)
	{
		if(this->report_stations[i]->GetUID() == topic_data->GetId())
		{
			report_station = this->report_stations[i];
			break;
		}
	}

	if(report_station != nullptr)
	{
		topic_data->GetName(report_stations[active_report_stations]->GetName());
		report_station->SetReportPeroid(topic_data->GetReportPeroid());
		report_station->SetLastReportedValue(
			topic_data->GetValue(mqtt_topic::DefaultTopicValues::Temperature),
			mqtt_topic::DefaultTopicValues::Temperature);
		report_station->SetLastReportedValue(
			topic_data->GetValue(mqtt_topic::DefaultTopicValues::Humidity),
			mqtt_topic::DefaultTopicValues::Humidity);
		this->Service();
	}
	else
	{
		// nie mamy jeszcze takiego urzadzenia wiec je zapisujemy o ile mamy miejsce
		if(this->active_report_stations < constants::max_report_stations &&
		   this->report_stations[active_report_stations] != nullptr)
		{
			report_stations[active_report_stations]->SetUID(topic_data->GetId());
			topic_data->GetName(report_stations[active_report_stations]->GetName());
			report_stations[active_report_stations]->SetReportPeroid(topic_data->GetReportPeroid());
			report_stations[active_report_stations]->SetLastReportedValue(
				topic_data->GetValue(mqtt_topic::DefaultTopicValues::Temperature),
				mqtt_topic::DefaultTopicValues::Temperature);
			report_stations[active_report_stations]->SetLastReportedValue(
				topic_data->GetValue(mqtt_topic::DefaultTopicValues::Humidity),
				mqtt_topic::DefaultTopicValues::Humidity);
			this->active_report_stations++;
			this->Service();
		}
		else
		{
			Serial.println("Report station list is full !!!");
		}
	}
}

void CentralHeatingWorkController::Service()
{
	uint8_t last_id = 0;
	if(this->active_report_stations > 0)
	{
		last_id = this->active_report_stations - 1;
		Serial.println("");
		Serial.print("Got data from report station: \"");
		Serial.print(this->report_stations[last_id]->GetName());
		Serial.print("\", uid: ");
		Serial.print(this->report_stations[last_id]->GetUID(), DEC);
		Serial.print(" , peroid: ");
		Serial.print(this->report_stations[last_id]->GetReportPeroid(), DEC);
		Serial.print(" , temperature: ");
		Serial.print(this->report_stations[last_id]->GetLastReportedValue(
						 mqtt_topic::DefaultTopicValues::Temperature),
					 2);
		Serial.print(" , humidity: ");
		Serial.print(this->report_stations[last_id]->GetLastReportedValue(
						 mqtt_topic::DefaultTopicValues::Humidity),
					 2);
		Serial.println("");

		double temperatures[constants::max_report_stations];
		for(size_t i{}; i < this->active_report_stations; i++)
		{
			temperatures[i] = this->report_stations[i]->GetLastReportedValue(
				mqtt_topic::DefaultTopicValues::Temperature);
		}
		this->temperature_algorithm->CompareSetpointWithValues(
			this->setpoint, temperatures, this->active_report_stations);
	}
}