#pragma once

namespace misc
{
class ICloudProvider
{
public:
	virtual void Service() = 0;
	virtual void SetValueToSend(bool value) = 0;
};

} // namespace misc