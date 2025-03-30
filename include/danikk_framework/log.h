#pragma once

#include<danikk_framework/danikk_framework.h>
#include<danikk_framework/format.h>
#include<danikk_framework/datetime.h>
#include<danikk_framework/filestream.h>
#include<danikk_framework/thread.h>
#include<danikk_framework/trace.h>

namespace danikk_framework
{
	extern Mutex logger_mutex;
	extern FileWriter log_file;
	extern String log_buffer;

	extern bool enable_memory_log;

	struct LogConfig
	{
		bool log_to_file = false;
		bool log_to_dir = true;
		bool log_to_cout = false;
		bool log_to_clog = true;

		bool print_time = true;
		bool use_system_time = false;
		bool use_application_time = true;
	};

	extern LogConfig log_config;

	void configureLog();

	namespace internal
	{
		extern const char* log_Debug;
		extern const char* log_Info;
		extern const char* log_Warning;
		extern const char* log_Error;
		extern const char* log_Fatal;
		extern const char* log_Trace;
		extern const char* log_Memory;
		extern const char* log_Assert;
	}

	bool startLog(const char* severity);

	void endLog();

	template<class arg_t> static void log(const char* severity, const arg_t& arg)
	{
		if(!startLog(severity)) return;
		log_buffer << arg;
		endLog();
	}

	template<class ...args_t> static void formatLog(const char* severity, const char* format, const args_t&... args)
	{
		if(!startLog(severity)) return;
		formatWrite(log_buffer, format, args...);
		endLog();
	}

	#define DEFINE_LOG_FUNCTION(severity)\
	template<class arg_t> inline static void log ## severity(const arg_t& arg){log(internal::log_##severity, arg);}\
	template<class ...args_t> inline static void formatLog ## severity(const char* format,const args_t&... args){formatLog(internal::log_##severity, format, args...);}\
	template<class ...args_t> inline static bool startLog ## severity(){ return startLog(internal::log_##severity);}

	DEFINE_LOG_FUNCTION(Debug)
	DEFINE_LOG_FUNCTION(Info)
	DEFINE_LOG_FUNCTION(Warning)
	DEFINE_LOG_FUNCTION(Error)
	DEFINE_LOG_FUNCTION(Fatal)
	DEFINE_LOG_FUNCTION(Trace)
	DEFINE_LOG_FUNCTION(Memory)
	DEFINE_LOG_FUNCTION(Assert)
	#undef DEFINE_LOG_FUNCTION

	#define LOG_OPERATOR(input) template<class log_t> inline log_t& operator <<(log_t& out, input)
}
