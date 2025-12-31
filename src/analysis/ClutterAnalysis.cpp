#include "ClutterAnalysis.h"
#include <ctime>
#include "FileTypeUtils.h"

std::vector<AnalysisResult> ClutterAnalyzer::analyze(
    const std::vector<FileInfo> &files) {

        std::vector<AnalysisResult> results; // store the results for the files passed

        std::time_t now = std::time(nullptr);

        const std::time_t UNUSED_DAYS = 90LL * 24 * 60 * 60; // 90 days
        const std::time_t OLD_DAYS    = 180LL * 24 * 60 * 60; // 180 days
        const std::uintmax_t LARGE_SIZE = 50ULL * 1024 * 1024; // 50 MB

        for(const auto& file: files){

            if(file.isDeleted || file.isDirectory){
                /// skip deleted or directories
                continue;
            }

            // unused files
            if (file.lastAccess != 0 &&
            (now - file.lastAccess) > UNUSED_DAYS) {

            results.push_back({
                file.path,
                ClutterReason::unused,
                classifyFileType(file.path),
                static_cast<long long> (file.size)
            });
            continue;
         }

        // OLD FILES
        if ((now - file.lastModified) > OLD_DAYS) {

            results.push_back({
                file.path,
                ClutterReason::old,
                classifyFileType(file.path),
                static_cast<long long> (file.size)
            });
            continue;
        }

        // LARGE FILES
        if (file.size > LARGE_SIZE) {

            results.push_back({
                file.path,
                ClutterReason::large,
                classifyFileType(file.path),
                static_cast<long long> (file.size)
            });
        }
            
            
        }

        return results;
    }

std::vector<AnalysisResult> ClutterAnalyzer::filterByFileType(
    const std::vector<AnalysisResult>& results,
    FileType type) {

    std::vector<AnalysisResult> filtered;

    for (const auto& r : results) {
        if (r.filetype == type) {
            filtered.push_back(r);
        }
    }

    return filtered;
}
