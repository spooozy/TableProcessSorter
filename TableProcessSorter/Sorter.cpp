#include "Sorter.h"



Sorter::Sorter(std::wstring folderPath) {
    this->folderPath = folderPath;
}

//std::wstring Sorter::ConvertFileTimeToDateString(const FILETIME& fileTime) {
//    SYSTEMTIME st;
//    FileTimeToSystemTime(&fileTime, &st);
//    wchar_t buffer[100];
//    /*std::swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%02d.%02d.%04d",
//        st.wDay, st.wMonth, st.wYear);*/
//    std::swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%04d.%02d.%02d",
//        st.wYear, st.wMonth, st.wDay);
//    return buffer;
//}

//
//void Sorter::ProcessDirectory(const std::wstring& dirPath) {
//    /* WIN32_FIND_DATA findFileData;
//    HANDLE hFind = FindFirstFile((dirPath + L"\\*").c_str(), &findFileData);
//
//    if (hFind == INVALID_HANDLE_VALUE) {
//        return;
//    }
//    do {
//        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
//            FileInfo info;
//            info.fileName = findFileData.cFileName;
//            info.fileExtension = info.fileName.substr(info.fileName.find_last_of(L"."));
//            info.creationDate = ConvertFileTimeToDateString(findFileData.ftCreationTime);
//            filesInfo.push_back(info);
//        }
//        else if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
//            ProcessDirectory(dirPath + L"\\" + findFileData.cFileName);
//        }
//    } while (FindNextFile(hFind, &findFileData) != 0);
//
//    FindClose(hFind);*/
//   
//
//    
//}

std::wstring Sorter::ConvertFileTimeToDateString(const FILETIME& fileTime) {
    SYSTEMTIME systemTime;
    FileTimeToSystemTime(&fileTime, &systemTime);

    std::wstringstream ss;
    ss << std::setw(4) << systemTime.wYear << L'.'
        << std::setw(2) << std::setfill(L'0') << systemTime.wMonth << L'.'
        << std::setw(2) << std::setfill(L'0') << systemTime.wDay;
    return ss.str();
}

// Функция для получения времени создания файла
FILETIME Sorter::GetCreationTime(const std::wstring& path) {
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &fileInfo)) {
        return fileInfo.ftCreationTime;
    }
    throw std::runtime_error("Не удалось получить атрибуты файла.");
}

void Sorter::ProcessDirectory(const std::wstring& dirPath) {
    try {
        for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(dirPath)) {
            if (std::filesystem::is_regular_file(entry.status())) {
                std::wstring fullFilename = entry.path().filename().wstring();
                std::wstring fileExtension = entry.path().extension().wstring();
                
                // Получаем время создания файла
                FILETIME creationTime = GetCreationTime(entry.path().wstring());
                std::wstring creationDate = ConvertFileTimeToDateString(creationTime);

                FileInfo fileInfo = { fullFilename, fileExtension, creationDate };
                filesInfo.push_back(fileInfo);
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
    }
    catch (const std::runtime_error& e) {
    }
}



void Sorter::swap(std::vector<FileInfo>& files, size_t first, size_t second) {
    auto temp = files[first];
    files[first] = files[second];
    files[second] = temp;
}

void Sorter::sortNames(std::vector<FileInfo>& files, size_t start, size_t end) {
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

void Sorter::sortExt(std::vector<FileInfo>& files, size_t start, size_t end) {
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

void Sorter::sortDate(std::vector<FileInfo>& files, size_t start, size_t end) {
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

void Sorter::WriteFilesToFiles(const std::vector<FileInfo>& files, int numParts) {
    int totalFiles = files.size();
    int filesPerPart = totalFiles / numParts;
    int remainder = totalFiles % numParts;

    for (int i = 0; i < numParts; ++i) {
        std::wstring fileName = L"D:\\БГУИР\\3 курс\\СП\\TableProcessSorter\\tempFiles\\temp_file_" + std::to_wstring(i + 1) + L".txt";
        std::wofstream outFile(fileName);
        outFile.imbue(std::locale(""));

        if (!outFile.is_open()) {
            return;
        }

        int currentPartSize = filesPerPart + (i < remainder ? 1 : 0);
        for (int j = 0; j < currentPartSize; ++j) {
            int fileIndex = i * filesPerPart + min(i, remainder) + j;
            const FileInfo& fileInfo = files[fileIndex];
            outFile << fileInfo.fileName << std::endl;
            outFile << fileInfo.fileExtension << std::endl;
            outFile << fileInfo.creationDate << std::endl;
            outFile << std::endl;
        }

        outFile.close();
    }
}

void Sorter::ReadFilesFromParts(int numParts, std::vector<FileInfo>& files) {
    for (int i = 0; i < numParts; ++i) {
        std::wstring fileName = L"D:\\БГУИР\\3 курс\\СП\\TableProcessSorter\\tempFiles\\temp_file_" + std::to_wstring(i + 1) + L".txt";
        std::wifstream inFile(fileName);
        inFile.imbue(std::locale(""));

        if (!inFile.is_open()) {
            continue;
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
}

void Sorter::ProcessCompleted(HANDLE processHandle, int index, int sortCase) {
    std::wstring fileName = L"D:\\БГУИР\\3 курс\\СП\\TableProcessSorter\\tempFiles\\temp_file_" + std::to_wstring(index) + L".txt";
    std::wifstream inFile(fileName);
    inFile.imbue(std::locale(""));

    if (!inFile.is_open()) {
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
        filesInfoSorted.push_back(info);
    }
    inFile.close();
    switch (sortCase) {
    case 1: {
        sortNames(filesInfoSorted, 0, filesInfoSorted.size() - 1);
        break;
    }
    case 2: {
        sortExt(filesInfoSorted, 0, filesInfoSorted.size() - 1);
        break;
    }
    case 3: {
        sortDate(filesInfoSorted, 0, filesInfoSorted.size() - 1);
        break;
    }
    default:
        break;
    }
}

std::vector<Sorter::FileInfo> Sorter::getData() {
    return filesInfo;
}

std::vector<Sorter::FileInfo> Sorter::getSortedData() {
    return filesInfoSorted;
}

void Sorter::readDir() {
    ProcessDirectory(folderPath);
    WriteFilesToFiles(filesInfo, procNum);
}

void Sorter::sort(int sortCase) {
    std::vector<bool> flags;
    std::vector<HANDLE> processHandles;
    std::vector<FileInfo> newVecor;
    procFinishedCounter = 0;
    filesInfoSorted = newVecor;
    
    for (int i = 0;i < procNum;i++)
        flags.push_back(false);    

    for (int i = 0; i < procNum; ++i) {
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_SHOW;

        std::wstring command = L"D:\\БГУИР\\3 курс\\СП\\TableProcessSorter\\x64\\Debug\\SortingProcess.exe " + std::to_wstring(i + 1) + L" " + std::to_wstring(sortCase);

        if (CreateProcess(NULL, &command[0], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            processHandles.push_back(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else {
            DWORD errorCode = GetLastError();
        }
    }

    while (procFinishedCounter < procNum - 1) {
        for (size_t i = 0; i < processHandles.size(); i++) {
            if(flags[i]!=true)
            {
                DWORD waitResult = WaitForSingleObject(processHandles[i], 0);

                if (waitResult == WAIT_OBJECT_0) {
                    ProcessCompleted(processHandles[i], i + 1, sortCase);
                    CloseHandle(processHandles[i]);
                    flags[i] = true;
                    procFinishedCounter++;
                }
            }

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }    
}