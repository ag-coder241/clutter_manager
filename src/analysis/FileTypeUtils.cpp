#include "FileTypeUtils.h"
#include <algorithm>

static std::string getExtension(const std::string& path) {
    auto pos = path.find_last_of('.');
    if (pos == std::string::npos) return "";
    std::string ext = path.substr(pos + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

FileType classifyFileType(const std::string& path) {
    std::string ext = getExtension(path);

    if (ext == "pdf") return FileType::PDF;

    if (ext == "jpg" || ext == "jpeg" || ext == "png" ||
        ext == "gif" || ext == "bmp" || ext == "webp")
        return FileType::Image;

    if (ext == "mp4" || ext == "mkv" || ext == "avi" || ext == "mov")
        return FileType::Video;

    if (ext == "mp3" || ext == "wav" || ext == "aac")
        return FileType::Audio;

    if (ext == "zip" || ext == "rar" || ext == "7z" || ext == "tar")
        return FileType::Archive;

    if (ext == "doc" || ext == "docx" || ext == "txt" || ext == "ppt")
        return FileType::Document;

    return FileType::Other;
}
