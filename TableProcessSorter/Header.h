#pragma once
#include <windows.h>
#include <string>
#include <vector>

class Header {
public:
    Header(int tableColsNum);
    void addHeaderItem(std::wstring text, int id, int x, int width);
    void drawHeader(HDC hdc);  
    int getHeaderItemClickedId(int x, int y);
    ~Header();

private:

    struct headerItem {
        std::wstring text;
        int x;
        int width;
    };

    std::vector<headerItem> headerItems;
    HWND parentWindow;
    HDC hdc;
    int id, tableColsNum, height = 40;
    static int colNum;
    
};
