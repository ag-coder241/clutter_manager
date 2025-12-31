#include "FileScanner.h"
#include <filesystem>
#include <iostream>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

std::vector<FileInfo> FileScanner::scanDirectory(const std::string& rootPath){
    std::vector<FileInfo> results;
    scanRecursive(rootPath, results);
    return results;
}

bool FileScanner::shouldIgnore(const std::string& path) const {
    static const std::vector<std::string> ignorePatterns = {
        "/.git",
        "/node_modules",
        "/Library/Caches",
        "/Library/Logs"
    };

    for (const auto& pattern : ignorePatterns) {
        if (path.find(pattern) != std::string::npos) {
            return true;
        }
    }
    return false;
}

void FileScanner::scanRecursive(const std::string& path, std::vector<FileInfo>& results){

    if(shouldIgnore(path)){
        return;
    }
    try{
        for(const auto& entry : fs::directory_iterator(path)){

            // recursive scanning

            try{
            FileInfo info;
            info.path = entry.path().string();

            fs::file_status status = entry.symlink_status();

            info.isSymLink = fs::is_symlink(status);
            info.isDirectory = fs::is_directory(status);

            if(!info.isDirectory && !info.isSymLink){
                info.size = entry.file_size();
            } else{
                info.size = 0;
            }

            auto ftime = entry.last_write_time(); // last modified time

            // time conversion from OS to C++ compatible
            auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                ftime - fs::file_time_type::clock::now()
                + std::chrono::system_clock::now()
            );

            info.lastModified = std::chrono::system_clock::to_time_t(sctp);

            info.lastAccess = info.lastModified; // for now

            results.push_back(info);

            // recurse if it's a directory and not a symlink

            if(info.isDirectory && !info.isSymLink){
                scanRecursive(info.path, results);

            }
        } catch (const fs::filesystem_error&){
            // skip this entry
            continue;
        }
        }
        } catch(const fs::filesystem_error& e){
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }

}


