#pragma once

#include "communication/WiFiAggregator.h"
#include "misc/DataWrapper.h"
#include "misc/ICloudProvider.h"

namespace misc
{
class AskStationsCloud : public misc::ICloudProvider
{
private:
	data::DataWrapper& data_storage;
	communication::WiFiAggregator& wifi_aggregator;

	bool send_data_flag;
	bool value;

public:
	AskStationsCloud(data::DataWrapper& data_storage,
					 communication::WiFiAggregator& wifi_aggregator);
	virtual void Service() override;
	virtual void SetValueToSend(bool value) override;
};

} // namespace misc