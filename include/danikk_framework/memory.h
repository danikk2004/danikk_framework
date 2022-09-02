#ifndef DANIKK_PLATFORM_MEMORY_H
#define DANIKK_PLATFORM_MEMORY_H

#include <danikk_framework/danikk_framework.h>
#include <cstring>

namespace danikk_framework
{
	constexpr size_t kyloBytes(size_t value){return value * 1024;}
	constexpr size_t megaBytes(size_t value){return value * 1024 * 1024;}
	constexpr size_t gigaBytes(size_t value){return value * 1024 * 1024 * 1024;}

	template<class valueT> inline valueT& createNullRef()
	{
		return (valueT&)*(valueT*)NULL;
	}

	template<class valueT> inline bool isNullRef(valueT& ref)
	{
		return (void*)&ref == NULL;
	}

	template<class valueT> inline void nullifyObjectMemory(valueT& ref)
	{
		memset(&ref, 0, sizeof(valueT));
	}

	template<class valueT> inline void objectToDefault(valueT& ref)
	{
		new (&ref) valueT();
	}

	template<class valueT> inline valueT* structmalloc()
	{
		return (valueT*)::malloc(sizeof(valueT));
	}

	template<class classT, class argsT> inline void callConstructor(classT& object, argsT args...)
	{
		new (&object) classT(args);
	}
}

#endif
