#include "Database.h"
#include <iostream>

Database::Database(const std::string& dbPath): db(nullptr), path(dbPath){}; // constructor

Database::~Database(){
    // destructor
    if(db){
        sqlite3_close(db);
    }
    // if db is opened close it 
    // to save resources
}

bool Database::initialize(){

    // open database
    if(sqlite3_open(path.c_str(), &db) != SQLITE_OK){
        std::cerr << "Failed to open database: "
                << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // SQL query
    const std::string createTableSQL = 
        "CREATE TABLE IF NOT EXISTS files ("
        "path TEXT PRIMARY KEY, "
        "size INTEGER NOT NULL, "
        "last_modified INTEGER NOT NULL, "
        "is_directory INTEGER NOT NULL, "
        "is_symlink INTEGER NOT NULL, "
        "last_scanned INTEGER NOT NULL"
        ");"; 
    
    return execute(createTableSQL);
}

bool Database::execute(const std::string& sql){
    char* errMsg = nullptr;

    if(sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK){
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool Database::fileExists(const std::string &path, std::int64_t& size, std::int64_t& last_modified){

    const char* sql = "SELECT size, last_modified FROM files WHERE path = ?;";

    sqlite3_stmt* stmt = nullptr;

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK){
        // we use prepare statements here for our SQL query
        return false;
    }

    sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_TRANSIENT);
    // add the file path to the SQL query

    bool exists = false;

    if(sqlite3_step(stmt) == SQLITE_ROW){
        size = sqlite3_column_int64(stmt, 0);
        last_modified = sqlite3_column_int64(stmt, 1);
        exists = true;
    }

    sqlite3_finalize(stmt);
    return exists;
}


bool Database::upsertFile(const FileInfo& file) {
    // sync the file with the database
    std::int64_t dbSize = 0;
    std::int64_t dbLastModified = 0;

    std::time_t now = std::time(nullptr); // when was this file accessed (current time)

    if(!fileExists(file.path, dbSize, dbLastModified)){
        // if it doesn't exists 
        const char* insertSQL = 
            "INSERT INTO files "
            "(path, size, last_modified, is_directory, is_symlink, last_scanned) "
            "VALUES (?, ?, ?, ?, ?, ?);";
        
        sqlite3_stmt* stmt = nullptr;

        // use prepared statements 
        if(sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }

        // insert values 
        sqlite3_bind_text(stmt, 1, file.path.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int64(stmt, 2, file.size);
        sqlite3_bind_int64(stmt, 3, file.lastModified);
        sqlite3_bind_int(stmt, 4, file.isDirectory ? 1 : 0);
        sqlite3_bind_int(stmt, 5, file.isSymLink ? 1 : 0);
        sqlite3_bind_int64(stmt, 6, now);

        // the numbering in the binding statement has nothing to
        // do with columns, it just the location of the placeholders
        // in the SQL statements

        sqlite3_step(stmt);
        sqlite3_finalize(stmt); // free 
        return true;
    }

    // if the file exits 
    // we have to check if it has changed

    if(dbSize == static_cast<std::int64_t>(file.size) &&
        dbLastModified == static_cast<std::int64_t>(file.lastModified)){
            // file is unchanged
            // just update last scanned

            const char* SQL =
                "UPDATE files SET last_scanned = ? WHERE path = ?;";
            
            sqlite3_stmt* stmt = nullptr;
            sqlite3_prepare_v2(db, SQL, -1, &stmt, nullptr);

            sqlite3_bind_int64(stmt, 1, now);
            sqlite3_bind_text(stmt, 2, file.path.c_str(), -1, SQLITE_TRANSIENT);

            sqlite3_step(stmt);
            sqlite3_finalize(stmt); // free 
            return true;

        }

        const char* updateSQL = 
            "UPDATE files SET "
            "size = ?, last_modified = ?, is_directory = ?, is_symlink = ?, last_scanned = ? "
            "WHERE path = ?;";

        sqlite3_stmt* stmt = nullptr;

        // use prepared statements 
        sqlite3_prepare_v2(db, updateSQL, -1, &stmt, nullptr);

        // insert values 
        sqlite3_bind_int64(stmt, 1, file.size);
        sqlite3_bind_int64(stmt, 2, file.lastModified);
        sqlite3_bind_int(stmt, 3, file.isDirectory ? 1 : 0);
        sqlite3_bind_int(stmt, 4, file.isSymLink ? 1 : 0);
        sqlite3_bind_int64(stmt, 5, now);
        sqlite3_bind_text(stmt, 6, file.path.c_str(), -1, SQLITE_TRANSIENT);

        sqlite3_step(stmt);
        sqlite3_finalize(stmt); // free 
        return true;
}