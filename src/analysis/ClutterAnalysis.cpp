#include "ClutterAnalysis.h"
#include <ctime>

std::vector<AnalysisResult> ClutterAnalyzer::analyze(
    const std::vector<FileInfo> &files) {

        std::vector<AnalysisResult> results; // store the results for the files passed

        std::time_t now = std::time(nullptr);

        const std::time_t sixMonths = 
            6LL * 60 * 60 * 24 * 30;

        for(const auto& file: files){
            
            if(file.isDeleted){
                results.push_back({
                    file.path,
                    ClutterReason::deletedFile,
                    static_cast<long long> (file.size)
                });
                continue;
            }

            if(!file.isDirectory && file.size > 100LL * 1024 * 1024
                && (now - file.lastModified) > sixMonths){

                    results.push_back({
                        file.path,
                        ClutterReason::oldFile,
                        static_cast<long long> (file.size)
                    });
                }
        }

        return results;
    }