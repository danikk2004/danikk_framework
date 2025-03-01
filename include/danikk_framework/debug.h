#pragma once
#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/localization.h>
#include <chrono>
#include <cfloat>
#include <algorithm>

namespace danikk_framework
{
	class Stopwatch
	{
	private:
		std::clock_t m_start;
	public:
		void start()
		{
			m_start = std::clock();
		}

		double elapsed()
		{
			return (std::clock() - m_start ) / (double)CLOCKS_PER_SEC;
		}
	};

	//Выполняет код n раз и выписывает минимальное/максимальное/среднее время выполнения.
	template <size_t n = 1000> void benchmark(auto someAction)
	{
		using std::min;
		using std::max;

		double averageTime = 0;
		double minTime = FLT_MAX;
		double maxTime = 0;

        Stopwatch watcher;

		for(size_t counter = 0; counter < n; counter++)
		{
			watcher.start();
			someAction();
			double elapsed = watcher.elapsed();
			minTime = min(minTime, elapsed);
			maxTime = max(maxTime, elapsed);
            averageTime += elapsed;
		}

        averageTime /= n;

		cout << localization("debug_max_time") << maxTime << endl;
		cout << localization("debug_min_time") << minTime << endl;
		cout << localization("debug_average_time") << averageTime << endl;
	}
}

#endif
