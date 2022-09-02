#ifndef DANIKK_PLATFORM_STREAM_H
#define DANIKK_PLATFORM_STREAM_H

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	class OutputStream
	{

	};
	class InputStream
	{

	};
	class IOStream : public OutputStream, public InputStream
	{

	};

#define WRITE_TO_STREAM_FUNC(STREAM_NAME) template<class streamT> void writeTo(STREAM_NAME& output) const
#define READ_FROM_STREAM_FUNC(STREAM_NAME)	template<class streamT> void readFrom(STREAM_NAME& input) const
}

#endif
