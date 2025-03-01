#pragma once
#include <danikk_framework/string.h>
#include <danikk_framework/trace.h>

namespace danikk_framework
{
	typedef enum
	{
		ok,
		cant_seek,
		file_not_opened

	} error_codes;

	class Exception
	{
		uint32 error_code;
	public:
		Exception(uint32 error_code) : error_code(error_code)
		{
			trace();
		}

		uint32 errorCode()
		{
			return error_code;
		}
	};

	class FileException : public Exception
	{
	private:
		uint32 errno_code;
	public:
		FileException(uint32 error_code, uint32 errno_code) : Exception(error_code)
		{
			this->errno_code = errno_code;
		}

		uint32 errnoCode()
		{
			return errno_code;
		}
	};
}
