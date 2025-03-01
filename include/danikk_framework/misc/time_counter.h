#include <danikk_framework/datetime.h>

namespace danikk_framework
{
	class TimeCounter
	{
	private:
		uint last_second = 0;
	    uint counter = 0;
	    uint value = 0;
	public:
		TimeCounter()
		{
			last_second = DateTime::local().second;
		}

		void check()
		{
			counter++;
            uint current_second = DateTime::local().second;
            if(last_second != current_second)
            {
            	last_second = current_second;
                value = counter;
                counter = 0;
            }
		}

		uint getValue()
		{
			return value;
		}
	};
}
