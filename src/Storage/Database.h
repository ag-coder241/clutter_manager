#pragma once

#include <string>
#include <sqlite3.h>
#include "../common/FileInfo.h"
#include <vector>

class Database{
public:
    explicit Database(const std::string& dbPath);
    ~Database(); // destructor

    bool initialize();

    bool upsertFile(const FileInfo& file); // given this file sync it with the db
    bool markDeletedFiles(std::time_t scanStart);
    std::vector<FileInfo> fetchAllFiles(); // fetch the files fromm the db

private:
    sqlite3* db; // pointer to db
    std::string path; // path to db

    bool execute(const std::string& sql);

    bool fileExists(const std::string &path, std::int64_t& size, std::int64_t& last_modified);
};