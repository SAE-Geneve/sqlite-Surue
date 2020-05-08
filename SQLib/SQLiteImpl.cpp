#include "SQLiteImpl.h"

#include <stdexcept>
#include <algorithm>
#include <sstream>

namespace sql {
static int Callback(void* ptr, const int ac, char** av, char** columnName)
{
    auto* sqliteImpl = static_cast<SQLiteImpl*>(ptr);

    row_t column(ac);

    for (int i = 0; i < ac; i++) {
        std::string val = av[i];
        column[i] = {columnName[i], sqliteImpl->GetValue(val)};
    }

    sqliteImpl->table_.push_back(column);

    return 0;
}

SQLiteImpl::SQLiteImpl(const std::string& file)
{
    if (sqlite3_open(&file[0], &db_) != 0) {
        throw std::runtime_error("error while opening db");
    }
}

SQLiteImpl::~SQLiteImpl() { sqlite3_close(db_); }

bool SQLiteImpl::ExecuteString(const std::string& cmd)
{
    //Clearing previous values
    error_.clear();
    table_.clear();

    //Pointer to store the error
    char* errMsg;

    //Execute command
    if (sqlite3_exec(db_, &cmd[0], Callback, this, &errMsg) != 0) {
        error_ = errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

value_t SQLiteImpl::GetValue(const std::string& str) const
{
    //Test if is null.
    if (str.empty()) { return nullptr; }

    std::istringstream ss(str);

    //Test if is an int.
    int64_t i;
    ss >> i;
    if (!ss.fail() && ss.eof()) { return i; }

    //Test if is a double.
    double d;
    ss >> d;
    if (!ss.fail() && ss.eof()) { return d; }

    //It's a string
    return str;
}
} // namespace sql