#pragma once
#include <vector>
#include "../common/FileInfo.h"
#include "AnalysisResults.h"

class ClutterAnalyzer{
public:
    std::vector<AnalysisResult> analyze(
        const std::vector<FileInfo>& files
    );
};

