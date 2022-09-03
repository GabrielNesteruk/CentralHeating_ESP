#pragma once

namespace configuration
{
template <typename T>
class IConfiguration
{
public:
	virtual bool Save() const = 0;
	virtual bool Load() = 0;
	virtual void Clear() const = 0;
	virtual void Update(const T& data) = 0;
	virtual T Get() const = 0;
};
} // namespace configuration