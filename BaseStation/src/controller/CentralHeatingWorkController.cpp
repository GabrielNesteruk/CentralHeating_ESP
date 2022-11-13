#include "controller/CentralHeatingWorkController.h"
#include "Constants.h"
#include "communication/DefaultTopicDataParser.h"

using namespace controller;

static void printReportStationInfo(device::ReportStation<double>* report_station)
{
	if(report_station != nullptr)
	{
		Serial.println("");
		Serial.print("Got data from report station: \"");
		Serial.print(report_station->GetName());
		Serial.print("\", uid: ");
		Serial.print(report_station->GetUID(), DEC);
		Serial.print(" , peroid: ");
		Serial.print(report_station->GetReportPeroid(), DEC);
		Serial.print(" , temperature: ");
		Serial.print(
			report_station->GetLastReportedValue(mqtt_topic::DefaultTopicValues::Temperature), 2);
		Serial.print(" , humidity: ");
		Serial.print(report_station->GetLastReportedValue(mqtt_topic::DefaultTopicValues::Humidity),
					 2);
		Serial.println("");
	}
}

CentralHeatingWorkController::CentralHeatingWorkController(
	device::ReportStation<double>* report_stations_array,
	algorithm::IAlgorithm<double>* temperature_algorithm,
	double default_setpoint,
	data::DataWrapper& data_storage,
	lcd::Lcd& lcd,
	device::Relay& relay,
	misc::AppState& appState)
	: setpoint{default_setpoint}
	, temperature_algorithm{temperature_algorithm}
	, data_storage{data_storage}
	, lcd{lcd}
	, relay{relay}
	, appState{appState}
{
	for(size_t i{}; i < constants::max_report_stations; i++)
	{
		this->report_stations[i] = &report_stations_array[i];
	}
}

device::ReportStation<double>* CentralHeatingWorkController::getReportStationByUID(uint8_t uid)
{
	for(size_t i{}; i < constants::max_report_stations; i++)
	{
		if(this->report_stations[i]->GetUID() == uid)
		{
			return this->report_stations[i];
		}
	}

	return nullptr;
}

void CentralHeatingWorkController::UpdateReportStation(mqtt_topic::ITopicData<double>* topic_data)
{

	device::ReportStation<double>* report_station = getReportStationByUID(topic_data->GetId());
	this->last_saved_uid = topic_data->GetId();

	if(report_station != nullptr)
	{
		String tmpName;
		topic_data->GetName(tmpName);
		lcd.UpdatePrintableElement(
			topic_data->GetId(),
			tmpName,
			topic_data->GetValue(mqtt_topic::DefaultTopicValues::Temperature),
			topic_data->GetValue(mqtt_topic::DefaultTopicValues::Humidity));

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
			String tmpString;
			topic_data->GetName(tmpString);

			lcd.AddPrintableElement(lcd::Lcd::PrintableElement(
				topic_data->GetId(),
				tmpString,
				topic_data->GetValue(mqtt_topic::DefaultTopicValues::Temperature),
				topic_data->GetValue(mqtt_topic::DefaultTopicValues::Humidity)));

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
			lcd.PrintError(definitions::ERROR_TYPE::REPORT_STATION_FULL_LIST_ERROR);
		}
	}
}

void CentralHeatingWorkController::Service()
{
	auto report_station = getReportStationByUID(this->last_saved_uid);

	if(report_station != nullptr)
	{
		printReportStationInfo(report_station);

		double temperatures[constants::max_report_stations];
		double avg = 0.0;
		for(size_t i{}; i < this->active_report_stations; i++)
		{
			temperatures[i] = this->report_stations[i]->GetLastReportedValue(
				mqtt_topic::DefaultTopicValues::Temperature);

			avg += temperatures[i];
		}
		bool algorithm_result = this->temperature_algorithm->CompareSetpointWithValues(
			this->setpoint, temperatures, this->active_report_stations);

		this->data_storage.getAvgTemperature() =
			avg / static_cast<double>(this->active_report_stations);

		if(algorithm_result)
		{
			relay.On();
			appState.SetState(misc::AppStates::HEATING);
		}
		else
		{
			relay.Off();
			appState.SetState(misc::AppStates::COOLING);
		}
	}
}

void CentralHeatingWorkController::UpdateInternalValues()
{
	this->setpoint = data_storage.getSetTemperature();
}