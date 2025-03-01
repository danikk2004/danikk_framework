#include<danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	template <class value_t> class TriangleWaveGen
	{
	private:
		value_t value;
		value_t min;
		value_t max;
		value_t delta;
	public:
		TriangleWaveGen(value_t value, value_t min, value_t max, value_t delta)
		{
			this->value = value;
			this->min = min;
			this->max = max;
			this->delta = delta;
		}

		value_t get()
		{
			value_t result = value;
			value+=delta;
			if(value <= min)
			{
				value = min;
				delta = -delta;
			}
			else if(value >= max)
			{
				value = max;
				delta = -delta;
			}
			return result;
		}

		value_t peek()
		{
			return value;
		}

		void setValue(value_t value)
		{
			this->value = value;
		}


		void setMax(value_t max)
		{
			this->max = max;
		}
	};
}
