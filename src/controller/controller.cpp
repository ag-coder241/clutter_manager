#include "controller.h"
#include "../scanner/FileScanner.h"
#include "../storage/Database.h"
#include "../analysis/ClutterAnalysis.h"

#include <iostream>
#include <filesystem>

Controller::Controller(const std::string& rootPath) : rootPath(rootPath) {}; // constructor

void Controller::runScan() {

    std::time_t scanStart = std::time(nullptr);

    std::cout << "Starting scan at: " << rootPath << std::endl;

    FileScanner scanner;
    auto files = scanner.scanDirectory(rootPath);

    std::filesystem::path dbPath = std::filesystem::current_path().parent_path() / "data" / "clutter.db";
    Database db(dbPath.string());// making an object with path to the db

    if(!db.initialize()){
        std::cerr << "Database initialisation Failed";
        return;
    }

    for(const auto& file: files){
        db.upsertFile(file);
    }

    db.markDeletedFiles(scanStart);

    std::cout << "Scan completed. Files processed: "
              << files.size() << std::endl;

    auto allFiles = db.fetchAllFiles(); // fetch files from the db for analysis

    ClutterAnalyzer analyzer;
    auto results = analyzer.analyze(allFiles);

    if(results.size()){
    std::cout << "Clutter candidates found: "
          << results.size() << std::endl;
    } else{
    std::cout << "No clutter candidates found" << std::endl;
    }

   for (const auto& r : results) {
    std::cout << "- " << r.path
              << " | size: " << r.size
              << " | reason: ";

    switch (r.reason) {
        case ClutterReason::deletedFile:
            std::cout << "Deleted file";
            break;
        case ClutterReason::oldFile:
            std::cout << "Old file";
            break;
        case ClutterReason::largeFile:
            std::cout << "Large file";
            break;
    }

    std::cout << std::endl;
}


}