#pragma once

#include <string>

enum class ClutterReason {
    deletedFile,
    oldFile,
    largeFile
};

struct AnalysisResult{
    std::string path;
    ClutterReason reason;
    long long size;
};