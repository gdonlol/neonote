#include "FileHandler.h"
#include <fstream>

void FileHandler::setBasePath(const std::string& path) {
    basePath = path;
}

std::vector<std::string> FileHandler::loadFile(const std::string& filename) {
    std::vector<std::string> lines;
    std::string path = basePath + "/" + filename + ".md";
    
    std::ifstream file(path);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    
    if (lines.empty()) {
        lines.push_back("");
    }
    
    return lines;
}

void FileHandler::saveFile(const std::string& filename, const std::vector<std::string>& lines) {
    std::string path = basePath + "/" + filename + ".md";
    
    std::ofstream file(path);
    for (const auto& line : lines) {
        file << line << '\n';
    }
}

bool FileHandler::createFile(const std::string& filename) {
    std::string path = basePath + "/" + filename;
    std::ofstream file(path);
    return file.good();
}
