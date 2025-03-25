#include "FileManager.h"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>

using namespace std;

FileManager::FileManager() {
    const char *home = getenv("HOME");
    if (home == nullptr) {
        throw runtime_error("No home directory found");
    }
    appDataPath = string(home) + "/.local/share/neonote";
    initializeAppDirectory();
    scanExistingFiles();
    createDefaultFileIfNeeded();
}

void FileManager::initializeAppDirectory() {
    struct stat info;
    if (!(stat(appDataPath.c_str(), &info) == 0 && S_ISDIR(info.st_mode))) {
        if (mkdir(appDataPath.c_str(), 0775) != 0) {
            throw runtime_error("Failed to create application directory");
        }
    }
}

void FileManager::scanExistingFiles() {
    for (const auto &entry : filesystem::directory_iterator(appDataPath)) {
        files.push_back(entry.path().stem().string());
    }
}

void FileManager::createDefaultFileIfNeeded() {
    if (files.empty()) {
        string filePath = appDataPath + "/Untitled1.md";
        ofstream file(filePath);
        if (!file) {
            throw runtime_error("Default file creation failed");
        }
        file.close();
        files.push_back("Untitled1");
    }
}

vector<string> FileManager::getFiles() const {
    return files;
}

void FileManager::loadFile(const string &filename, vector<string> &lines) {
    string path = appDataPath + "/" + filename + ".md";
    ifstream file(path);
    string line;
    
    lines.clear();
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
}

void FileManager::saveFile(const string &filename, const vector<string> &lines) {
    string path = appDataPath + "/" + filename + ".md";
    ofstream file(path);
    for (const auto &line : lines) {
        file << line << '\n';
    }
    file.close();
}
