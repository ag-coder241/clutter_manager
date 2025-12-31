#pragma once

#include <string>
#include <sqlite3.h>
#include "../common/FileInfo.h"

class Database{
public:
    explicit Database(const std::string& dbPath);
    ~Database(); // destructor

    bool initialize();

    bool upsertFile(const FileInfo& file); // given this file sync it with the db

private:
    sqlite3* db; // pointer to db
    std::string path; // path to db

    bool execute(const std::string& sql);

    bool fileExists(const std::string &path, std::int64_t& size, std::int64_t& last_modified);
};