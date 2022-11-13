#include "device/PushButton.h"

using namespace device;

static constexpr uint8_t pin_number = D1;
static volatile bool _isPressed = false;

void IRAM_ATTR btnPressFnc()
{
	if(digitalRead(pin_number) == HIGH)
	{
		_isPressed = false;
	}
	else
	{
		_isPressed = true;
	}
}

PushButton::PushButton(configuration::ConfigurationManager& config_manager)
	: config_manager{config_manager}
{
	pinMode(pin_number, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pin_number), btnPressFnc, CHANGE);
}

void PushButton::Service()
{
	if(lock)
	{
		if((millis() - locked_time >= 3000) && _isPressed)
		{
			config_manager.Clear();
			ESP.reset();
		}
		else if(!_isPressed)
			lock = false;
	}
	else
	{
		if(_isPressed)
		{
			lock = true;
			locked_time = millis();
		}
	}
}