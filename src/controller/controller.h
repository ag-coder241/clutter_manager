#pragma once

#include <string>

class Controller{
public:
    explicit Controller(const std::string& rootPath);
    void runScan();

private:
    std::string rootPath;
};