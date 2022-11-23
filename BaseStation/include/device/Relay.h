#pragma once

#include <Arduino.h>

namespace device
{
class Relay
{
private:
	static constexpr uint8_t pin_number = 22;

public:
	Relay()
	{
		pinMode(pin_number, OUTPUT);
		digitalWrite(pin_number, LOW);
	}

	void On()
	{
		if(digitalRead(pin_number) == LOW)
		{
			digitalWrite(pin_number, HIGH);
		}
	}

	void Off()
	{
		if(digitalRead(pin_number) == HIGH)
		{
			digitalWrite(pin_number, LOW);
		}
	}
};

} // namespace device