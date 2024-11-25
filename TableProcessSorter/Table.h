#pragma once
#include "Cell.h"
#include "Header.h"
#include <vector>
#include "Sorter.h"

class Table {
public:
	HWND parentWindow;	HDC hdc;
	int tableWidth, tableHeight;

	Table(HWND parentWindow, int tableWidth, int tableHeight, int rows, int cols);
	~Table();

	void drawTable(HDC hdc, int scrollPos);
	int rowsLeft();	
	void updateTableParams(int newTableWidth, int newTableHeight);
	void handleClick(int clickPosX, int clickPosY);
	void fillTable(std::wstring dir);
	int getWidth1();
	int getHeaderHeight();

private:
	
	Sorter* sorter = nullptr;
	Header* header = nullptr;
	struct FileInfo {
		std::wstring fileName;
		std::wstring fileExtension;
		std::wstring creationDate;
	};

	std::vector<Sorter::FileInfo> filesInfo;
	std::vector<Sorter::FileInfo> filesInfoSorted;

	std::vector<std::vector<Cell>>cells;
	int rows, cols, cellWidth, cellHeight, scrollPos = 0, visibleRows, visibleCols, minCellWidth = 10, minCellHeight = 20, headerHeight = 40, difY = 0;
	float k1 = 0.3, k2 = 0.1, k3 = 0.1;
	void createCells();	
	void setVisibleCells();
	void updateCellsParams();
	void drawTableBorders(HDC hdc);	
};
