#pragma once

#include <vector>
#include <string>
#include "../common/FileInfo.h"

class FileScanner {
public:
    std::vector<FileInfo> scanDirectory(const std::string& directoryPath);

private:
    void scanRecursive(const std::string& path, std::vector<FileInfo>& results);

    bool shouldIgnore(const std::string& path) const;
};

