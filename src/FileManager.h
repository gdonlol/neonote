#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <vector>
#include <string>

class FileManager {
public:
    FileManager();
    std::vector<std::string> getFiles() const;
    void loadFile(const std::string &filename, std::vector<std::string> &lines, std::string &current_file);
    void saveFile(const std::string &filename, const std::vector<std::string> &lines);
    void newFile();
    void deleteFile(const std::string &filename);
    void renameFile(const std::string &filename, std::string newName, std::string &current_file);
    
private:
    std::string appDataPath;
    std::vector<std::string> files;
    
    void initializeAppDirectory();
    void scanExistingFiles();
    void createDefaultFileIfNeeded();
};

#endif
