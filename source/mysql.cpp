#include <danikk_framework/mysql.h>

#if HAS_MYSQL

namespace danikk_framework
{
	MySQL::MySQL()
	{
		connection_is_opened = false;
		mysql_init(&connection);
	}

	MySQL::~MySQL()
	{
		if(connection_is_opened)
		{
			mysql_close(&connection);
		}
	}

	error_code MySQL::open(const char *host, const char *user, const char *passwd, const char *db)
	{
		return (error_code)mysql_real_connect(&connection, host, user, passwd, db, 0, NULL, 0);
	}

	void MySQL::close()
	{
		if(connection_is_opened)
		{
			connection_is_opened = false;
			mysql_close(&connection);
		}
	}

	error_code MySQL::query(const char* request)
	{
		error_code result = mysql_query(&connection, tocstring(request));

		if(result)
		{
			last_error_buffer.clear();
			last_error_buffer << "Причина:" << mysql_error(&connection) << endl;
			last_error_buffer << "Запрос:" << request << endl;
		}

		return result;
	}

	const char* MySQL::error()
	{
		return last_error_buffer.c_string();
	}

	using Result = MySQL::Result;

	Result MySQL::getResult()
	{
		return Result(*this);
	}

	Result::Result(MySQL& connection)
	{
		result = mysql_store_result(&connection.connection);
	}

	Result::~Result()
	{
		mysql_free_result(result);
	}

	Result::operator bool()
	{
		MYSQL_ROW row = mysql_fetch_row(result);
		return row[0][0] == '1';
	}

	using Iterator = Result::Iterator;

	Iterator::Iterator(MYSQL_RES* result)
	{
		this->result = result;
		next();
	}

	Iterator::operator bool()
	{
		return last != NULL;
	}

	MYSQL_ROW Iterator::next()
	{
		MYSQL_ROW ret = last;

		last = mysql_fetch_row(result);
		return ret;
	}

	Iterator Result::begin()
	{
		return Iterator(result);
	}
}

#endif
