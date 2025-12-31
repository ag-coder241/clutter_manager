#include "controller.h"
#include "../scanner/FileScanner.h"
#include "../storage/Database.h"

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

}