# pragma once

#include <string>
#include <cstdint>
#include <ctime>
#include <stdio.h>

struct FileInfo {
    std::string path;
    std::uintmax_t size; // file size in bytes
    std::time_t lastAccess; 
    std::time_t lastModified;
    bool isDirectory = false;
    bool isSymLink = false;
    bool isDeleted = false;
};