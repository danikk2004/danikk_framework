#pragma once

#define HAS_MYSQL __has_include(<mysql/mysql.h>)

//#if not HAS_MYSQL
//	#error cant find mysql.h, install mysqlclient
//#endif

#include <mysql/mysql.h>
#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/string.h>
#include "cstring_functions.h"
#include <danikk_framework/format.h>
#include <danikk_framework/log.h>
#include <danikk_framework/assert.h>

#include <compile_config.h>

#include <mutex>

namespace danikk_framework
{
	struct MySQLConnectionInfo
	{
		String user;
		String passwd;
		String db;
		String host;
		uint32 port = 0;
		String charset = "utf8";
	};

	class MySQL
	{
	private:
		MYSQL connection;
		size_t connection_is_opened;
		String request_buffer;
		String last_error_buffer;
	public:
		MySQLConnectionInfo connection_info;
	private:

		#if MYSQL_USE_MUTEX
			std::mutex* mutex;
		#endif

		#if MYSQL_USE_MUTEX
			#define MYSQL_LOCK mutex->lock();
			#define MYSQL_UNLOCK mutex->unlock();
		#else
			#define MYSQL_LOCK
			#define MYSQL_UNLOCK
		#endif

		typedef MySQL this_t;
	public:
		MySQL()
		{
			#if MYSQL_USE_MUTEX
				mutex = new std::mutex();
			#endif
			connection_is_opened = false;
			mysql_init(&connection);
		}

		~MySQL()
		{
			#if MYSQL_USE_MUTEX
				if (mutex != NULL)
				{
					delete mutex;
				}
			#endif
			if(connection_is_opened)
			{
				mysql_close(&connection);
				connection_is_opened = false;
			}
		}

		int open()
		{
			assert(!connection_is_opened);
			formatLogInfo("% %", "Подключение к базе данных", connection_info.db);
			MYSQL* ptr = mysql_real_connect(&connection,
					connection_info.host.c_string(),
					connection_info.user.c_string(),
					connection_info.passwd.c_string(),
					connection_info.db.c_string(),
					connection_info.port,
					NULL,
					0);
			if(ptr != NULL)
			{
				mysql_set_character_set(ptr, connection_info.charset.c_string());
				connection_is_opened = true;
				return 0;
			}
			else
			{
				return 1;
			}
		}

		void openNecessarily()
		{
			if(open())
			{
				formatLogFatal("% user:% passwd:% db:% host:% port:% charset:%",
					"Не удалось подключиться к базе данных",
					connection_info.user,
					connection_info.passwd,
					connection_info.db,
					connection_info.host,
					connection_info.port,
					connection_info.charset);
				abort();
			}
		}

		void close()
		{
			assert(connection_is_opened);
			connection_is_opened = false;
			mysql_close(&connection);
		}

		void try_close()
		{
			if(connection_is_opened)
			{
				close();
			}
		}

		template<class format_t, class ...args_t> int query(const format_t& format_str, const args_t&... args)
		{
			MYSQL_LOCK
			format(request_buffer, tocstring(format_str), args...);
			int result = query(request_buffer.c_string());
			return result;
		}
	private:
		template<class strT> int query_internal(strT& str)
		{
			return query_internal(tocstring(str));
		}

		int query_internal(const char* request)
		{
			try_close();
			int result = open();
			if(result)
			{
				last_error_buffer.clear();
				last_error_buffer << formatCreate("% user:% passwd:% db:% host:% port:% charset:%",
						"Не удалось подключиться к базе данных",
						connection_info.user,
						connection_info.passwd,
						connection_info.db,
						connection_info.host,
						connection_info.port,
						connection_info.charset);
				last_error_buffer << "Причина:" << result << endl;
				return result;
			}
			formatLogInfo("SQL Запрос : %", request);
			result = mysql_query(&connection, request);

			if(result)
			{
				format(last_error_buffer, "Ошибка:%", mysql_error(&connection));
			}

			return result;
		}
	public:

		template<class strT> int query(strT& str)
		{
			MYSQL_LOCK
			return query_internal(str);
		}

		int query(const char* request)
		{
			MYSQL_LOCK
			return query_internal(request);
		}

		template<class str_t> void logError(cref<str_t> message)
		{
			formatLogError("%\n%", message, last_error_buffer);
			MYSQL_UNLOCK
		}

		void logFatal(const char* message)
		{
			formatLogFatal("%\n%", message, last_error_buffer);
			MYSQL_UNLOCK
		}

		void confirm()
		{
			MYSQL_UNLOCK
		}

		class Result
		{
		private:
			MYSQL_RES* result = NULL;

			friend class BaseMySQL;

			Result() = default;
		public:
			Result(this_t& connection)
			{
				result = mysql_store_result(&connection.connection);
			}

			~Result()
			{
				if(result != NULL)
				{
					mysql_free_result(result);
				}
			}

			bool isNull()
			{
				return result == NULL;
			}

			operator bool()
			{
				MYSQL_ROW row = mysql_fetch_row(result);
				return row != NULL && row[0] != NULL && row[0][0] == '1';
			}

			carray<ccstring> getData()
			{
				Result::Iterator iterator = begin();
				carray<ccstring> data = iterator.next();
				return move(data);
			}

			class Iterator
			{
			private:
				MYSQL_RES* result;
				MYSQL_ROW last;
			public:
				Iterator(MYSQL_RES* result)
				{
					this->result = result;
					next();
				}

				operator bool()
				{
					return last != NULL;
				}

				carray<ccstring> next()
				{
					if(last == NULL)
					{
						return NULL;
					}
					else
					{
						MYSQL_ROW ret = last;
						last = mysql_fetch_row(result);
						return (carray<ccstring>)ret;
					}
				}
			};

			Iterator begin()
			{
				assert(result != NULL);
				return Iterator(result);
			}
		};

	private:
		template<class str_t, class value_t> int resultToArray(ref<DynamicArray<value_t>> array, cref<str_t> query)
		{
			this_t::Result result = getResult();
			if(result.isNull())
			{
				format(last_error_buffer, "Ошибка:result == null");
				return *((int*)"reqn");
			}
			typename this_t::Result::Iterator iter = result.begin();
			array.clear();
			while(iter)
			{
				ccstring* value = iter.next();
				array.pushCtor(value);
			}
			return 0;
		}
	public:

		template<class value_t> inline int querySelectToArray(
				const char* query_string,
				ref<DynamicArray<value_t>> array)
		{
			MYSQL_LOCK
			int error = query_internal(query_string);
			if(error)
			{
				return error;
			}
			return resultToArray(array, query_string);
		}

		template<class value_t> inline int querySelectToArrayWhere(cref<String> table, cref<String> where, cref<DynamicArray<ccstring>> values, ref<DynamicArray<value_t>> array)
		{
			int error = querySelectWhere(table, where, values);
			if(error)
			{
				return error;
			}
			return resultToArray(array, request_buffer);
		}

		int querySetValuesWhere(cref<String> table, cref<String> where, cref<InitList<std::pair<ccstring, String>>> values)
		{
			MYSQL_LOCK
			request_buffer.clear();
			formatAppend(request_buffer, "UPDATE % SET ", table);
			formatAppendList(request_buffer, ",", values.begin(), values.end(),
				[&](cref<std::pair<ccstring, String>> value)
				{
					return formatCreate("% = %" , value.first, single_quotes(value.second));
				}
			);
			formatAppend(request_buffer, " WHERE %;", where);
			return query_internal(request_buffer);
		}

		int querySelect(cref<String> table, cref<InitList<ccstring>> values)
		{
			MYSQL_LOCK
			format(request_buffer, "SELECT ");
			formatAppendList(request_buffer, ",", values.begin(), values.end());
			formatAppend(request_buffer, " FROM %;", table);
			return query_internal(request_buffer);
		}

		int querySelectWhere(cref<String> table, cref<String> where, cref<DynamicArray<ccstring>> values)
		{
			MYSQL_LOCK
			format(request_buffer, "SELECT ");
			formatAppendList(request_buffer, ",", values.begin(), values.end());
			formatAppend(request_buffer, "FROM % WHERE % ;", table, where);
			return query_internal(request_buffer);
		}

		int queryExitsWhere(cref<String> table, cref<String> where)
		{
			MYSQL_LOCK
			format(request_buffer, "SELECT EXISTS(SELECT * FROM % WHERE %);", table, where);
			return query_internal(request_buffer);
		}

		template<class value_t> int queryExitsWhereColunmEqual(cref<String> table, cref<String> colunm, cref<value_t> value)
		{
			MYSQL_LOCK
			format(request_buffer, "SELECT EXISTS(SELECT * FROM % WHERE % = %);", table, colunm, single_quotes(value));
			return query_internal(request_buffer);
		}

		template<class value_t> int querySELECTWhereColunmEqual(cref<String> table, cref<String> colunm, cref<String> select, cref<value_t> value)
		{
			MYSQL_LOCK
			format(request_buffer, "SELECT % FROM % WHERE % = %;", select, table, colunm, single_quotes(value));
			return query_internal(request_buffer);
		}

		int queryInsertInto(cref<String> table, cref<InitList<std::pair<ccstring, String>>> values)
		{
			MYSQL_LOCK
			format(request_buffer, "INSERT INTO % (", table);
			formatAppendList(request_buffer, ",", values.begin(), values.end(),
				[&](cref<std::pair<ccstring, String>> value)
				{
					return value.first;
				}
			);
			formatAppend(request_buffer, ") VALUES (");
			formatAppendList(request_buffer, ",", values.begin(), values.end(),
				[&](cref<std::pair<ccstring, String>> value)->cref<String>
				{
				return value.second;
				}
			);
			formatAppend(request_buffer, ");");
			return query_internal(request_buffer);
		}

		int queryGetLastId(const char* table, const char* id_name = "id")
		{
			MYSQL_LOCK
			format(request_buffer, "SELECT MAX(%) FROM %;", id_name, table);
			return query_internal(request_buffer);
		}

		friend class Result;

		Result getResult()
		{
			MYSQL_UNLOCK
			return Result(*this);
		}

		#undef MYSQL_LOCK
		#undef MYSQL_UNLOCK
	};

	/*static bool verifyMySQLColumnCount(const char** row)
	{
		while(row)
	}*/
}
