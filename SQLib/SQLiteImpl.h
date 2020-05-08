#pragma once

#include "SQLib.h"
#include <sqlite3.h>

namespace sql {

	class SQLiteImpl : public SQLib
	{
	public:
		SQLiteImpl(const std::string& file);
		virtual ~SQLiteImpl();

		bool ExecuteString(const std::string& cmd) override;

	public:
		const std::string& GetError() const override { return error_; }
		const table_t& GetReturnValues() const override { return table_; }

	protected:
		value_t GetValue(const std::string& str) const;

	private:
		table_t table_ = {};
		sqlite3* db_ = nullptr;
		std::string error_;
		friend static int Callback(
			void* ptr, 
			int ac, 
			char** av, 
			char** column_name);
	};

} // End namespace sql.
