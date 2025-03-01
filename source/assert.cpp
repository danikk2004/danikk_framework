#include <danikk_framework/log.h>
#include <danikk_framework/string.h>
#include <danikk_framework/assert.h>

namespace danikk_framework
{
	void assertFail(
			const char* expr,
			const char* file,
			int line,
			const char* func)
	{
		String err;
		trace();
		danikk_framework::format(err, "ASSERT ERROR %:%:% %", line, file, func, expr);
		throw err;
	}
}
