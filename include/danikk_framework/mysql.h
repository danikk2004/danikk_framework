#pragma once

#define HAS_MYSQL __has_include(<mysql/mysql.h>)

#if not HAS_MYSQL
	#error cant find mysql.h, install mysqlclient
#endif

#include <mysql/mysql.h>
#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/string.h>
#include <danikk_framework/cstring.h>
#include <danikk_framework/cout_features.h>
#include <danikk_framework/format.h>

namespace danikk_framework
{
	class MySQL
	{
	private:
		MYSQL connection;
		bool connection_is_opened;
		String request_buffer;
		String last_error_buffer;
	public:
		MySQL();

		~MySQL();

		error_code open(const char *host = NULL, const char *user = NULL, const char *passwd = NULL, const char *db = NULL);

		void close();

		error_code query(const char* request);

		template<class format_t, class ...args_t> error_code query(const format_t& format_str, const args_t&... args)
		{
			format(request_buffer, tocstring(format_str), args...);
			error_code result = query(request_buffer.c_string());
			return result;
		}

		template<class strT> error_code query(strT& str)
		{
			error_code result = mysql_real_query(&connection, str.data(), str.size());
			last_error_buffer.clear();
			last_error_buffer << "Причина:" << mysql_error(&connection) << endl;
			last_error_buffer << "Команда:" << request_buffer << endl;
			return result;
		}

		template<class strT> MySQL& operator <<(const strT& data)
		{
			request_buffer << data;
			return *this;
		}

		const char* error();

		class Result
		{
		private:
			MYSQL_RES* result;

			friend class MySQL;

			Result(MySQL& connection);
		public:
			~Result();

			operator bool();

			class Iterator
			{
			private:
				MYSQL_RES* result;
				MYSQL_ROW last;
			public:
				Iterator(MYSQL_RES* result);

				operator bool();

				MYSQL_ROW next();
			};

			Iterator begin();

			/*template<class iter_t> void foreach(iter_t iter)
			{
				MYSQL_ROW current_row;

				while ((current_row = mysql_fetch_row(result)))
				{
					iter(current_row);
				}
			}*/
		};

		friend class Result;

		Result getResult();

		void logError(const char* message);
	};
}
