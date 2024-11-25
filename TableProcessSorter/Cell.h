#pragma once
#include <windows.h>
#include <string>

class Cell {
public:
    std::wstring text;    
    int x, y, width, height;
    Cell(std::wstring text);
    ~Cell();
    void draw(HDC hdc);    

private:
      
};
