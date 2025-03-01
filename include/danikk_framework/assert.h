#pragma once

namespace danikk_framework
{
	void assertFail(
			const char* expr,
			const char* file,
			int line,
			const char* func);
}

#if defined(assert)
	#undef assert
#endif
#define assert(expr) if(!(bool)(expr)) danikk_framework::assertFail(#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__)

#define assertGoto(expr, label) if(!(bool)(expr)) { goto label; }

#define assertWarnGoto(expr, label) if(!(bool)(expr)) { logAssert(#expr); goto label; }

#define assertRet(expr, ret) if(!(bool)(expr)) { return ret; }

#define assertWarnRet(expr, ret) if(!(bool)(expr)) { logAssert(#expr); return ret; }
