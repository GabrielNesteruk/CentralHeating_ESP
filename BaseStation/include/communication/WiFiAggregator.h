#pragma once

#include "IConfiguration.h"
#include "WorkFlowController.h"
#include "communication/MQTT.h"
#include "configuration/ConfigurationData.h"
#include "configuration/ConfigurationManager.h"
#include "definitions.h"
#include "device/Lcd.h"
#include "device/PushButton.h"
#include <ArduinoJson.h>
#include <WebServer.h>
#include <WiFi.h>
#include <misc/DataWrapper.h>

namespace communication
{
class WiFiAggregator
{
private:
	WiFiClass& _WiFi;
	configuration::ConfigurationManager& config_manager;
	WebServer server;
	const IPAddress basic_ip_address; // basic ip address assigned for AP
	const IPAddress basic_gateway_address; // basic gateway address assigned for AP
	const IPAddress basic_mask; // basic mask assigned for AP
	MQTT mqtt;
	data::DataWrapper& data_storage;
	controller::WorkFlowController<double>* work_flow_controller;
	lcd::Lcd& lcd;
	device::PushButton& pushButton;

	void WaitForConfigData();
	void SetServerEndpoints();
	void ExtractIpOctetsFromString(const char* str, uint8_t* array);

public:
	WiFiAggregator(WiFiClass& _WiFi,
				   configuration::ConfigurationManager& config_manager,
				   mqtt_topic::ITopicData<double>* topic_data,
				   controller::WorkFlowController<double>* work_flow_controller,
				   data::DataWrapper& data_storage,
				   lcd::Lcd& lcd,
				   device::PushButton& pushButton);

	void Init();

	void Service();

	void StopSendingData()
	{
		mqtt.Disconnect();
	}

	void StartSendingData()
	{
		mqtt.RawConnect();
	}
};

} // namespace communication