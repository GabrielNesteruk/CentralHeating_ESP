#pragma once

#include "ReportStation.h"

namespace algorithm
{

template <typename T>
class IAlgorithm
{
public:
	virtual bool CompareSetpointWithValues(T setpoint, T* reported_values, size_t size) = 0;
};

} // namespace algorithm