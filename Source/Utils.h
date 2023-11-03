#include <string>

static std::string ExtractBaseName(const std::string& path) {
    size_t pos = path.find_last_of('/');
    if (pos != std::string::npos) {
        return path.substr(pos + 1);
    } else {
        return path;
    }
}

static std::string ExtractExtension(const std::string&path){
    size_t pos = path.find_last_of('.');
    if (pos == std::string::npos) {
        return path.substr(pos);
    }
    return "";
}