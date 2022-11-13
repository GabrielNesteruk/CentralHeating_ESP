#pragma once

#include "configuration/ConfigurationManager.h"

namespace device
{
class PushButton
{
private:
	configuration::ConfigurationManager& config_manager;
	bool lock = false;
	unsigned int locked_time = 0;

public:
	PushButton(configuration::ConfigurationManager& config_manager);
	void Service();
};

} // namespace device