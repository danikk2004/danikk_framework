#include <danikk_framework/misc/countdown.h>

namespace danikk_framework
{
	Countdown::Countdown(uint32 start_value)
	{
		this->start_value = start_value;
		reset();
	}

	void Countdown::setStartValue(uint32 start_value)
	{
		this->start_value = start_value;
		reset();
	}

	void Countdown::reset()
	{
		value = start_value;
	}

	void Countdown::countDown()
	{
		if(value > 0)
		{
			value--;
		}
	}

	void Countdown::stop()
	{
		value = 0;
	}

	uint32 Countdown::getValue()
	{
		return value;
	}

	uint32 Countdown::getStartValue()
	{
		return start_value;
	}

	bool Countdown::isStart()
	{
		return value == start_value;
	}

	bool Countdown::isStop()
	{
		return value == 0;
	}
}
