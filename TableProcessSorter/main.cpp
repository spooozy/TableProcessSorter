#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "Table.h"

int clientWidth, clientHeight, scrollPos = 0;
HWND hWnd;
HWND dirInputWnd;
Table* table = nullptr;
std::wstring dir = L" ";

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void updateClientSizeParams();
void updateInputWndPos(HDC hdc);
std::chrono::system_clock::time_point clickTime;

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    const wchar_t TableClass[] = L"Table";
    int x = 0;


    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TableClass;
    wc.style = CS_DBLCLKS;
    RegisterClass(&wc);

    hWnd = CreateWindowEx(0, TableClass, L"Table", WS_OVERLAPPEDWINDOW | WS_VSCROLL,
        0, 0, screenWidth - 40, screenHeight,
        NULL, NULL, hInstance, NULL);

    updateClientSizeParams();   

    table = new Table(hWnd, clientWidth, clientHeight, 40, 6);
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);

    dirInputWnd = CreateWindowEx(0, L"EDIT", NULL,
        WS_VISIBLE | WS_CHILD | ES_CENTER,
        110, 13, 150, 20,
        hWnd, NULL, NULL, NULL);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN) {
            if ((x = GetWindowTextLength(dirInputWnd)) != 0){
                std::wstring direcory(x, L'\0');
                GetWindowText(dirInputWnd, &direcory[0], x + 1);
                dir = direcory;
                table->fillTable(direcory);
                InvalidateRect(hWnd, NULL, true);
            }
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT clearRect;
    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_CREATE:
    {
        break;
    }
    case WM_SIZE: {
        updateClientSizeParams();
        InvalidateRect(hWnd, NULL, true);
        break;
    }
    case WM_PAINT: {
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &clearRect);
        FillRect(hdc, &clearRect, (HBRUSH)(COLOR_WINDOW + 1));    
        table->updateTableParams(clientWidth, clientHeight);
        SetScrollRange(hWnd, SB_VERT, 0, table->rowsLeft(), true);
        updateInputWndPos(hdc);
        table->drawTable(hdc, scrollPos);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_COMMAND: {
        break;
    }   
    case WM_VSCROLL: {
        int oldPos = scrollPos;
        switch (LOWORD(wParam)) {
        case SB_LINEUP:
            if (--scrollPos < 0) scrollPos = 0;
            break;
        case SB_LINEDOWN:
            if (++scrollPos > table->rowsLeft())scrollPos = table->rowsLeft();
            break;
        }
        if (oldPos != scrollPos)
        {
            SetScrollPos(hWnd, SB_VERT, scrollPos, true);
            InvalidateRect(hWnd, NULL, true);
        }
        break;
    }
    case WM_LBUTTONDOWN: {        
        int clickPosX = LOWORD(lParam);
        int clickPosY = HIWORD(lParam);
        if(dir!=L" ")
        {
            table->handleClick(clickPosX, clickPosY);
            InvalidateRect(hWnd, NULL, true);
        }
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }   
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }
    return 0;
}

void updateClientSizeParams() {
    RECT rect;
    GetClientRect(hWnd, &rect);
    clientWidth = rect.right - rect.left;
    clientHeight = rect.bottom - rect.top;
}

void updateInputWndPos(HDC hdc) {
    if(dir!=L" ")
    {
        SIZE textSize;
        int width = table->getWidth1(), height = table->getHeaderHeight();
        GetTextExtentPoint32(hdc, dir.c_str(), dir.length(), &textSize);
        SetWindowPos(dirInputWnd, NULL, width / 2 - textSize.cx / 2, height / 2 - textSize.cy / 2, textSize.cx, textSize.cy, NULL);
    }
}