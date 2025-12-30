#include "FileScanner.h"
#include <filesystem>
#include <iostream>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;

std::vector<FileInfo> FileScanner::scanDirectory(const std::string& directoryPath){
    std::vector<FileInfo> results;
    try{
        for(const auto& entry : fs::directory_iterator(directoryPath)){
            // non recursive scanning
            FileInfo info;
            info.path = entry.path().string();
            info.isDirectory = entry.is_directory();

            if(!info.isDirectory){
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

            info.lastAcess = info.lastModified; // for now

            results.push_back(info);
        }
        } catch(const fs::filesystem_error& e){
            std::cerr << "Filesystem error: " << e.what() << std::endl;
        }

    return results;
}