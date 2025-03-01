#pragma once
#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	class Countdown
	{
	private:
		uint32 value = 0;
		uint32 start_value;
	public:
		Countdown() = default;

		Countdown(uint32 start_value);

		void setStartValue(uint32 start_value);

		void reset();//сброс value в max_value

		void countDown();//уменьшение значения

		void stop();

		uint32 getValue();//получение значения

		uint32 getStartValue();

		bool isStart();

		bool isStop();
	};
}
