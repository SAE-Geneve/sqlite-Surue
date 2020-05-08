#include "SQLiteImpl.h"
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <sstream>

namespace sql {

	static int Callback(void* ptr, int ac, char** av, char** column_name) 
	{
		auto* sqliteImpl = static_cast<SQLiteImpl*>(ptr);

		column_t column(ac);

        for(int i = 0; i < ac; i++) {
			std::string val = av[i];
			column[i] = std::pair{ column_name[i], sqliteImpl->GetValue(val) };
        }

		sqliteImpl->table_.push_back(column);

		return 0;
	}

	SQLiteImpl::SQLiteImpl(const std::string& file)
	{
		if(sqlite3_open(&file[0], &db_) != 0) {
			throw std::runtime_error("error while opening db");
		}
	}

	SQLiteImpl::~SQLiteImpl()
	{
		sqlite3_close(db_);
	}

	bool SQLiteImpl::ExecuteString(const std::string& cmd)
	{
        //Clearing previous values
		error_.clear();
		table_.clear();

        //Pointer to store the error
		char* errMsg;

		if(sqlite3_exec(db_, &cmd[0], Callback, this, &errMsg) != 0) {
			error_ = errMsg;
			sqlite3_free(errMsg);
			return false;
		}
		return true;
	}

	sql::value_t SQLiteImpl::GetValue(const std::string& str) const
	{
        //Test if is null.
        if(str.empty()) {
			return nullptr;
        }

        //Test if is an int.
		std::istringstream ssi(str);

		int64_t i;
		ssi >> i;
		if (!ssi.fail() && ssi.eof()) {
			return i;
		}

        //Test if is a double.
		std::istringstream ssd(str);
		double d;
		ssd >> d;
        if(!ssd.fail() && ssd.eof()) {
			return d;
        }

        //It's a string
		return str;
	}

} // End namespace sql.
