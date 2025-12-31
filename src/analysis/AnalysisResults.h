#pragma once

#include <string>
#include "FileType.h"

enum class ClutterReason {
    unused,
    old,
    large
};

struct AnalysisResult{
    std::string path;
    ClutterReason reason;
    FileType filetype;
    long long size;
};