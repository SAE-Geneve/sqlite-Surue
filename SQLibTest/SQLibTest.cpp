#include "SQLibTest.h"
#include <variant>

namespace test {

TEST_F(SQLibTest, CreateSQLiteTest)
{
    EXPECT_FALSE(db_);
    // \" should be both illegal as a file name on windows and linux.
    EXPECT_THROW(db_ = sql::CreateSQLiteDB("\""), std::runtime_error);
    EXPECT_FALSE(db_);
    EXPECT_NO_THROW(db_ = sql::CreateSQLiteDB("./test.db"));
    EXPECT_TRUE(db_);
}

TEST_F(SQLibTest, SimpleCreateDropSQLiteTest)
{
    ASSERT_FALSE(db_);
    ASSERT_NO_THROW(db_ = sql::CreateSQLiteDB("./test.db"));
    ASSERT_TRUE(db_);
    // Execute empty string.
    EXPECT_TRUE(db_->ExecuteString(R"SQL()SQL"));
    // Execute drop test table.
    EXPECT_TRUE(db_->ExecuteString(R"SQL(DROP TABLE IF EXISTS test;)SQL"));
    // Execute drop table again without the if exist to check for errors.
    EXPECT_FALSE(db_->ExecuteString(R"SQL(DROP TABLE test;)SQL"));
    EXPECT_EQ("no such table: test", db_->GetError());
    // Execute create table test.
    EXPECT_TRUE(
        db_->ExecuteString(
            R"SQL(
			-- Create a new table test that include name and age of people.
			CREATE TABLE test (
				-- Created a primary key as an int autoincrement ascendant.
				id INTEGER PRIMARY KEY ASC AUTOINCREMENT,
				-- Name is a varchar.
				name VARCHAR(20),
				-- Family name is also a varchar.
				family VARCHAR(20),
				-- Age is an integer.
				age INTEGER DEFAULT 0
			);
		)SQL"
        ));
}

TEST_F(SQLibTest, SimpleInsertSQLiteTest)
{
    ASSERT_FALSE(db_);
    ASSERT_NO_THROW(db_ = sql::CreateSQLiteDB("./test.db"));
    ASSERT_TRUE(db_);
    // Execute drop test table.
    ASSERT_TRUE(db_->ExecuteString(R"SQL(DROP TABLE IF EXISTS test;)SQL"));
    // Execute create table test.
    ASSERT_TRUE(
        db_->ExecuteString(
            R"SQL(
			-- Create a new table test that include name and age of people.
			CREATE TABLE test (
				-- Created a primary key as an int autoincrement ascendant.
				id INTEGER PRIMARY KEY ASC AUTOINCREMENT,
				-- Name is a varchar.
				name VARCHAR(20),
				-- Family name is also a varchar.
				family VARCHAR(20),
				-- Age is an integer.
				age INTEGER DEFAULT 0
			);
		)SQL"
        ));
    EXPECT_TRUE(
        db_->ExecuteString(
            R"SQL(
			INSERT INTO test (name, family, age) VALUES
				("Bernard", "Kevin", 24),
				("John", "Kyle", 24),
				("Michael", "Kieran", 21),
				("Elizabeth", "Meghan", 21),
				("Rose", "Tara", 15),
				("Ann", "Rachel", 15);
		)SQL"
        ));
    {
        EXPECT_TRUE(db_->ExecuteString(R"SQL(
				SELECT COUNT(*) FROM test;
			)SQL"));
        auto table = db_->GetReturnValues();
        EXPECT_EQ(1, table.size());
    }
}

TEST_F(SQLibTest, SimpleQuerySQLiteTest)
{
    ASSERT_FALSE(db_);
    ASSERT_NO_THROW(db_ = sql::CreateSQLiteDB("./test.db"));
    ASSERT_TRUE(db_);
    // Execute drop test table.
    ASSERT_TRUE(db_->ExecuteString(R"SQL(DROP TABLE IF EXISTS test;)SQL"));
    // Execute create table test.
    ASSERT_TRUE(
        db_->ExecuteString(
            R"SQL(
			-- Create a new table test that include name and age of people.
			CREATE TABLE test (
				-- Created a primary key as an int autoincrement ascendant.
				id INTEGER PRIMARY KEY ASC AUTOINCREMENT,
				-- Name is a varchar.
				name VARCHAR(20),
				-- Family name is also a varchar.
				family VARCHAR(20),
				-- Age is an integer.
				age INTEGER DEFAULT 0
			);
		)SQL"
        ));
    ASSERT_TRUE(
        db_->ExecuteString(
            R"SQL(
			INSERT INTO test (name, family, age) VALUES
				("Bernard", "Kevin", 24),
				("John", "Kyle", 24),
				("Michael", "Kieran", 21),
				("Elizabeth", "Meghan", 21),
				("Rose", "Tara", 15),
				("Ann", "Rachel", 15),
				("Mary", "Rebecca", 15),
				("Peter", "Ryan", 15),
				("James", "Connor", 15),
				("Eric", "John", 15);
		)SQL"
        ));
    {
        EXPECT_TRUE(db_->ExecuteString(R"SQL(
				SELECT COUNT(*) FROM test;
			)SQL"));
        auto table = db_->GetReturnValues();
        EXPECT_EQ(1, table.size());
    }
    {
        EXPECT_TRUE(db_->ExecuteString(R"SQL(
				SELECT * FROM test;
			)SQL"));
        auto table = db_->GetReturnValues();
        EXPECT_EQ(10, table.size());
    }
    {
        EXPECT_TRUE(
            db_->ExecuteString(
                R"SQL(
				SELECT name, family, age FROM test WHERE age > 18;
			)SQL"));
        auto table = db_->GetReturnValues();
        EXPECT_EQ(4, table.size());
        for (const auto& column : table) { EXPECT_EQ(3, column.size()); }
    }
}

} // End namespace test.
