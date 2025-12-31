#pragma once
#include <vector>
#include "../common/FileInfo.h"
#include "AnalysisResults.h"

class ClutterAnalyzer{
public:
    std::vector<AnalysisResult> analyze(
        const std::vector<FileInfo>& files
    );

    std::vector<AnalysisResult> filterByFileType(
    const std::vector<AnalysisResult>& results,
    FileType type
    );
    // this function filters the analysis output by the filetype "type"

};

