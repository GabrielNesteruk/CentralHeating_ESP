#pragma once

#include <stddef.h>
#include <stdint.h>

namespace data_generator
{
class IDataGenerator
{
public:
	virtual uint16_t GenerateData(uint8_t* buffer, size_t buffer_length) = 0;
};

} // namespace data_generator