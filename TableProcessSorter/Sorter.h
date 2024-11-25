#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <iomanip>
#include <locale>
#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <filesystem>
#include <sstream>

class Sorter {
public:
    int sortCase = 2;
    struct FileInfo {
        std::wstring fileName;
        std::wstring fileExtension;
        std::wstring creationDate;
    };

    std::vector<FileInfo> filesInfo;
    std::vector<FileInfo> filesInfoSorted;
    std::wstring folderPath;

    Sorter(std::wstring folderPath);
    std::vector<FileInfo> getData();
    std::vector<FileInfo> getSortedData();

    void readDir();
    void sort(int sortCase);

private:
    int procNum = 4, procFinishedCounter = 0;
    
    std::wstring ConvertFileTimeToDateString(const FILETIME& fileTime);
    void ProcessDirectory(const std::wstring& dirPath);
    void swap(std::vector<FileInfo>& files, size_t first, size_t second);
    void sortNames(std::vector<FileInfo>& files, size_t start, size_t end);
    void sortExt(std::vector<FileInfo>& files, size_t start, size_t end);
    void sortDate(std::vector<FileInfo>& files, size_t start, size_t end);
    void WriteFilesToFiles(const std::vector<FileInfo>& files, int numParts);
    void ReadFilesFromParts(int numParts, std::vector<FileInfo>& files);
    void ProcessCompleted(HANDLE processHandle, int index, int sortCase);


    //std::wstring ConvertFileTimeToDateString(const FILETIME& fileTime);
    FILETIME GetCreationTime(const std::wstring& path);
};
