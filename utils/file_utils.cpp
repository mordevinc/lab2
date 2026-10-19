#include "file_utils.h"
#include <fstream>

#ifdef _WIN32
    #include <direct.h>
    #define MKDIR(path) _mkdir(path)
#else
    #include <sys/stat.h>
    #include <sys/types.h>
    #define MKDIR(path) mkdir(path, 0755)
#endif

void EnsureDirectory(const std::string& path) {
    MKDIR(path.c_str());
}

bool FileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

std::string BuildDataPath(const std::string& filename) {
    // если путь уже содержит "/" или "\" — считаем, что он задан явно
    if (filename.find('/') != std::string::npos ||
        filename.find('\\') != std::string::npos) {
        return filename;
    }
    return "data/" + filename;
}