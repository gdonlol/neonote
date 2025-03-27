#include "FileManager.h"
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>

using namespace std;

/**
 * @class FileManager
 * @brief The FileManager class is responsible for managing file operations such as loading, saving, 
 *        and creating files within the application directory.
 * 
 * It handles tasks like initializing the app directory, scanning for existing files, and ensuring 
 * that a default file is created if no files are present.
 */
FileManager::FileManager() {
    const char *home = getenv("HOME");
    if (home == nullptr) {
        throw runtime_error("No home directory found");
    }
    appDataPath = string(home) + "/.local/share/neonote";  /**< Application data path. */
    initializeAppDirectory();  /**< Initialize the app's directory if it doesn't exist. */
    scanExistingFiles();       /**< Scan for existing files in the app directory. */
    createDefaultFileIfNeeded(); /**< Create a default file if none exist. */
}

/**
 * @brief Initializes the application directory.
 * 
 * Checks if the application directory exists, and if not, attempts to create it. 
 * Throws an exception if the directory creation fails.
 */
void FileManager::initializeAppDirectory() {
    struct stat info;
    // Check if the directory exists and is actually a directory
    if (!(stat(appDataPath.c_str(), &info) == 0 && S_ISDIR(info.st_mode))) {
        // Attempt to create the directory with permissions 0775
        if (mkdir(appDataPath.c_str(), 0775) != 0) {
            throw runtime_error("Failed to create application directory");
        }
    }
}

/**
 * @brief Scans the application directory for existing files.
 * 
 * Iterates through the directory and adds the filenames (without extensions) to the `files` vector.
 */
void FileManager::scanExistingFiles() {
    for (const auto &entry : filesystem::directory_iterator(appDataPath)) {
        files.push_back(entry.path().stem().string());  /**< Extract and store filenames (without extension). */
    }
}

/**
 * @brief Creates a default file if no files are found in the application directory.
 * 
 * If no files are present in the app directory, this function creates a default file named "Untitled1.md" 
 * and adds it to the list of files.
 */
void FileManager::createDefaultFileIfNeeded() {
    if (files.empty()) {
        string filePath = appDataPath + "/Untitled1.md";  /**< Default file path. */
        ofstream file(filePath);  /**< Open the file for writing. */
        if (!file) {
            throw runtime_error("Default file creation failed");  /**< Throw error if file creation fails. */
        }
        file.close();  /**< Close the file after writing. */
        files.push_back("Untitled1");  /**< Add the default file to the list of files. */
    }
}

/**
 * @brief Retrieves the list of file names.
 * 
 * @return A vector of strings containing the names of all files in the application directory.
 */
vector<string> FileManager::getFiles() const {
    return files;  /**< Return the list of files. */
}

/**
 * @brief Loads the contents of a specified file into a vector of strings.
 * 
 * Opens the specified file, reads each line, and stores the lines in the provided `lines` vector.
 * 
 * @param filename The name of the file to load (without the ".md" extension).
 * @param lines A reference to the vector that will hold the lines of the file.
 */
void FileManager::loadFile(const string &filename, vector<string> &lines, std::string &current_file) {
    string path = appDataPath + "/" + filename + ".md";  /**< Construct the full file path. */
    ifstream file(path);  /**< Open the file for reading. */
    string line;
    current_file = filename;
    lines.clear();  /**< Clear any previous contents of the lines vector. */
    while (getline(file, line)) {
        lines.push_back(line);  /**< Add each line to the lines vector. */
    }
    file.close();  /**< Close the file after reading. */
}

/**
 * @brief Saves the contents of a vector of strings to a specified file.
 * 
 * Writes each string in the `lines` vector to a file with the given name. Each line is written 
 * followed by a newline character.
 * 
 * @param filename The name of the file to save (without the ".md" extension).
 * @param lines A reference to the vector containing the lines to write to the file.
 */
void FileManager::saveFile(const string &filename, const vector<string> &lines) {
    string path = appDataPath + "/" + filename + ".md";  /**< Construct the full file path. */
    ofstream file(path);  /**< Open the file for writing. */
    for (const auto &line : lines) {
        file << line << '\n';  /**< Write each line to the file. */
    }
    file.close();  /**< Close the file after writing. */
}

void FileManager::newFile(){
    int dupeId = 0;
    while(1){
        dupeId++;
        string path = appDataPath + "/Untitled" + std::to_string(dupeId) + ".md";
        ifstream file(path);
        if(!file.good()){
            files.push_back("Untitled" + std::to_string(dupeId));
            saveFile("Untitled" + std::to_string(dupeId), vector<string>{""});
            break;
        }
    }
}