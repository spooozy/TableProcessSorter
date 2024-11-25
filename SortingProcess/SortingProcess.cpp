#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <windows.h>

struct FileInfo {
    std::wstring fileName;
    std::wstring fileExtension;
    std::wstring creationDate;
};

void ReadFile(const std::wstring& filePath, std::vector<FileInfo>& files) {
    std::wifstream inFile(filePath);
    inFile.imbue(std::locale(""));

    if (!inFile.is_open()) {
        return;
    }

    std::wstring line;
    while (std::getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }

        FileInfo info;
        info.fileName = line;
        if (std::getline(inFile, line) && !line.empty()) {
            info.fileExtension = line;
        }
        if (std::getline(inFile, line) && !line.empty()) {
            info.creationDate = line;
        }
        files.push_back(info);
    }

    inFile.close();
}

void swap(std::vector<FileInfo>& files, size_t first, size_t second) {
    auto temp = files[first];
    files[first] = files[second];
    files[second] = temp;
}

void sortNames(std::vector<FileInfo>& files, size_t start, size_t end) {
    if (start >= end)
        return;
    size_t current = start;
    for (size_t i = start + 1; i <= end; i++) {
        if (files[i].fileName < files[start].fileName) {
            swap(files, ++current, i);
        }
    }
    swap(files, start, current);
    if (current > start) {
        sortNames(files, start, current - 1);
    }
    if (end > current + 1) {
        sortNames(files, current + 1, end);
    }
}

void sortExt(std::vector<FileInfo>& files, size_t start, size_t end) {
    if (start >= end)
        return;
    size_t current = start;
    for (size_t i = start + 1; i <= end; i++) {
        if (files[i].fileExtension < files[start].fileExtension) {
            swap(files, ++current, i);
        }
    }
    swap(files, start, current);
    if (current > start) {
        sortExt(files, start, current - 1);
    }
    if (end > current + 1) {
        sortExt(files, current + 1, end);
    }
}

void sortDate(std::vector<FileInfo>& files, size_t start, size_t end) {
    if (start >= end)
        return;
    size_t current = start;
    for (size_t i = start + 1; i <= end; i++) {
        if (files[i].creationDate < files[start].creationDate) {
            swap(files, ++current, i);
        }
    }
    swap(files, start, current);
    if (current > start) {
        sortDate(files, start, current - 1);
    }
    if (end > current + 1) {
        sortDate(files, current + 1, end);
    }
}

void WriteFile(const std::wstring& filePath, std::vector<FileInfo>& files) {
    std::wofstream outFile(filePath);
    outFile.imbue(std::locale(""));
    if (!outFile.is_open()) {
        return;
    }
    for (int i = 0;i < files.size();i++)
    {
        const FileInfo& fileInfo = files[i];
        outFile << fileInfo.fileName << std::endl;
        outFile << fileInfo.fileExtension << std::endl;
        outFile << fileInfo.creationDate << std::endl;
        outFile << std::endl;
    }
    outFile.close();
}

int main(int argc, char* argv[]) {
    //SetConsoleOutputCP(CP_UTF8);
   /* SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "Russian");*/

    if (argc < 3) {
        std::cout << "HELP" << std::endl;
        return 1;
    }
    int number = std::stoi(argv[1]);
    int sortCase = std::stoi(argv[2]);
    std::vector<FileInfo> filesInfo;
    std::wstring filePath = L"D:\\¡√”»–\\3 ÍÛÒ\\—œ\\FileProcessSorter\\tempFiles\\temp_file_" + std::to_wstring(number) + L".txt";
    ReadFile(filePath, filesInfo);
    switch (sortCase) {
    case 1: {
        sortNames(filesInfo, 0, filesInfo.size() - 1);
        break;
    }
    case 2: {
        sortExt(filesInfo, 0, filesInfo.size() - 1);
        break;
    }
    case 3: {
        sortDate(filesInfo, 0, filesInfo.size() - 1);
        break;
    }
    default:
        break;
    }
    WriteFile(filePath, filesInfo);
    return 0;
}