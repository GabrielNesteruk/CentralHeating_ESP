#pragma once

namespace misc
{

enum class AppStates
{
	INIT,
	HEATING,
	COOLING
};

class AppState
{
private:
	AppStates state;

public:
	AppState()
		: state(AppStates::INIT){};

	void SetState(AppStates state)
	{
		this->state = state;
	}
	AppStates GetState()
	{
		return this->state;
	}
};

} // namespace misc