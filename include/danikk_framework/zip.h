#pragma once

#if not __has_include(<zip.h>)
	#error cant find zip.h, install libzip
#endif

#include <zip.h>

namespace danikk_framework
{
	zip_t* zip_create_from_directory(const char* zipPath, const char* directory, int* errorp);

    const char* zip_error_code_tostr(int error);
}

#include <danikk_framework/internal/zip.inc>
