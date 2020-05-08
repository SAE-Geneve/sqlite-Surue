#pragma once

#include <gtest/gtest.h>
#include "../SQLib/SQLib.h"

namespace test {
class SQLibTest : public testing::Test {
public:
    SQLibTest() = default;

    std::string ValueToString(sql::value_t val) const
    {
        if (std::holds_alternative<std::nullptr_t>(val)) { return "nullptr"; }
        if (std::holds_alternative<double>(val)) { return std::to_string(std::get<double>(val)); }
        if (std::holds_alternative<std::int64_t>(val)) {
            return std::to_string(std::get<std::int64_t>(val));
        }
        return std::get<std::string>(val);
    }

protected:
    std::shared_ptr<sql::SQLib> db_ = nullptr;
};
} // namespace test
