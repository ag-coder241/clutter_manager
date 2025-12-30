#pragma once

#include <vector>
#include <string>
#include "../common/FileInfo.h"

class FileScanner {
public:
    std::vector<FileInfo> scanDirectory(const std::string& directoryPath);
};
