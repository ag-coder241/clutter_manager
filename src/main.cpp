// #include <iostream>
// #include "scanner/FileScanner.h"

// int main(){
//     FileScanner scanner; // making a object of the class

//     std::string path = std::getenv("HOME");
//     // path += "/Downloads"; // for testing purposes

//     auto files = scanner.scanDirectory(path);

//     for (const auto& file : files) {
//         std::tm* tm = std::localtime(&file.lastModified); // convert seconds to correct time

//         char buffer[64];
//         std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm); // format 

//         std::cout << file.path << " | "
//                   << file.size << " bytes | "
//                   << buffer << " | "
//                   << (file.isDirectory ? "DIR" : "FILE")
//                   << std::endl;
//     }

//     return 0;
// }


#include <iostream>
#include "storage/Database.h"
#include <filesystem>

int main() {

    std::filesystem::path dbPath = std::filesystem::current_path().parent_path() / "data" / "clutter.db";
    Database db(dbPath.string());// making an object with path to the db

    if (!db.initialize()) {
        std::cerr << "Database initialization failed\n";
        return 1;
    }

    std::cout << "Database initialized successfully\n";
    return 0;
}
