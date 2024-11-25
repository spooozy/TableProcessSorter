#include "Table.h"

Table::Table(HWND parentWindow, int tableWidth, int tableHeight, int rows, int cols) {
	this->parentWindow = parentWindow;
	this->tableWidth = tableWidth;
	this->tableHeight = tableHeight;
	this->rows = rows;
	this->cols = cols;
	//sorter = new Sorter(L"D:\\ÁÃÓÈÐ\\Äðóãîå");
	//sorter = new Sorter(L"D:\\ÁÃÓÈÐ\\3 êóðñ");
	//sorter = new Sorter(L"D:\\WorkSpace");
	//sorter = new Sorter(L"D:\\ìîðå");
	header = new Header(cols);	
	createCells();	
};

void Table::fillTable(std::wstring dir) {
	sorter = new Sorter(dir);
	sorter->readDir();
	filesInfo = sorter->getData();
	this->rows = filesInfo.size();
	createCells();
	for (int i = 0;i < rows;i++)
	{
		cells[i][0].text = filesInfo[i].fileName;
		cells[i][1].text = filesInfo[i].fileExtension;
		cells[i][2].text = filesInfo[i].creationDate;
	}
}

void Table::createCells() {
	cells.resize(rows, std::vector<Cell>(cols, Cell(L"")));
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			cells[i][j] = Cell(L" ");
		}
	}	
}

void Table::updateCellsParams()
{
	int width1 = (tableWidth - cols - 1) * k1;
	int width2 = (tableWidth - cols - 1) * k2;
	int width3 = (tableWidth - cols - 1) * k3;
	int x = 1, dif;

	difY = tableHeight - 1 - visibleRows * (cellHeight + 1) - headerHeight;

	for (int i = 0;i < visibleRows;i++) {
		x = 1;
		cells[i + scrollPos][0].x = x;
		cells[i + scrollPos][0].y = 1 + i * (cellHeight + 1) + headerHeight;
		cells[i + scrollPos][0].width = width1;
		cells[i + scrollPos][0].height = cellHeight;
		header->addHeaderItem(L" ", 0, x, width1);

		x = x + width1 + 1;

		cells[i + scrollPos][1].x = x;
		cells[i + scrollPos][1].y = 1 + i * (cellHeight + 1) + headerHeight;
		cells[i + scrollPos][1].width = width2;
		cells[i + scrollPos][1].height = cellHeight;
		header->addHeaderItem(L"Extension", 1, x, width2);

		x = x + width2 + 1;

		cells[i + scrollPos][2].x = x;
		cells[i + scrollPos][2].y = 1 + i * (cellHeight + 1) + headerHeight;
		cells[i + scrollPos][2].width = width3;
		cells[i + scrollPos][2].height = cellHeight;
		header->addHeaderItem(L"Date", 2, x, width3);

		x = x + width3 + 1;

		cells[i + scrollPos][3].x = x;
		cells[i + scrollPos][3].y = 1 + i * (cellHeight + 1) + headerHeight;
		cells[i + scrollPos][3].width = width1;
		cells[i + scrollPos][3].height = cellHeight;
		header->addHeaderItem(L"File Name", 3, x, width1);

		x = x + width1 + 1;

		cells[i + scrollPos][4].x = x;
		cells[i + scrollPos][4].y = 1 + i * (cellHeight + 1) + headerHeight;
		cells[i + scrollPos][4].width = width2;
		cells[i + scrollPos][4].height = cellHeight;
		header->addHeaderItem(L"Extension", 4, x, width2);

		x = x + width2 + 1;

		cells[i + scrollPos][5].x = x;
		cells[i + scrollPos][5].y = 1 + i * (cellHeight + 1) + headerHeight;
		cells[i + scrollPos][5].width = width3;
		cells[i + scrollPos][5].height = cellHeight;
		header->addHeaderItem(L"Date", 5, x, width3);
	}

	if (difY != 0)
	{
		dif = difY;
		for (int i = visibleRows - 1;i >= visibleRows - difY;i--) {
			for (int j = 0;j < visibleCols;j++)
			{
				cells[i + scrollPos][j].y += dif;
				cells[i + scrollPos][j].height++;
			}
			dif--;
		}
	}

}

void Table::setVisibleCells() {
	int tempHeight = (tableHeight - 1 - rows - headerHeight - 1) / rows;
	if (tempHeight < minCellHeight) cellHeight = minCellHeight;
	else cellHeight = tempHeight;

	visibleRows = (tableHeight - 1 - headerHeight - 1) / (cellHeight + 1);
	if (visibleRows > rows) visibleRows = rows;

	visibleCols = cols;
}

void Table::updateTableParams(int newTableWidth, int newTableHeight)
{
	tableWidth = newTableWidth;
	tableHeight = newTableHeight;
	setVisibleCells();	
}

void Table::drawTable(HDC hdc, int newScrollPos)
{
	scrollPos = newScrollPos;
	drawTableBorders(hdc);	
	updateCellsParams();
	header->drawHeader(hdc);
	for (int i = 0;i < visibleRows;i++) {
		for (int j = 0;j < cols;j++) {
			cells[i + scrollPos][j].draw(hdc);
		}
		MoveToEx(hdc, 0, cells[i+scrollPos][0].y + cells[i + scrollPos][0].height, nullptr);
		LineTo(hdc, tableWidth, cells[i + scrollPos][0].y + cells[i + scrollPos][0].height);
	}	
}

void Table::drawTableBorders(HDC hdc) {
	int x = 0, y = 0;
	MoveToEx(hdc, 0, 0, nullptr);
	LineTo(hdc, tableWidth, 0);

	MoveToEx(hdc, 0, tableHeight - 1, nullptr);
	LineTo(hdc, tableWidth, tableHeight - 1);

	MoveToEx(hdc, 0, 0, nullptr);
	LineTo(hdc, 0, tableHeight);

	MoveToEx(hdc, 0, headerHeight, nullptr);
	LineTo(hdc, tableWidth, headerHeight);

	MoveToEx(hdc, tableWidth - 1, 0, nullptr);
	LineTo(hdc, tableWidth - 1, tableHeight);

	x = 1 + (tableWidth - cols - 1) * k1;
	MoveToEx(hdc, x, 0, nullptr);
	LineTo(hdc, x, tableHeight);

	x = x + 1 + (tableWidth - cols - 1) * k2;
	MoveToEx(hdc, x, 0, nullptr);
	LineTo(hdc, x, tableHeight);

	x = x + 1 + (tableWidth - cols - 1) * k3;
	MoveToEx(hdc, x, 0, nullptr);
	LineTo(hdc, x, tableHeight);

	x = x + 1 + (tableWidth - cols - 1) * k1;
	MoveToEx(hdc, x, 0, nullptr);
	LineTo(hdc, x, tableHeight);

	x = x + 1 + (tableWidth - cols - 1) * k2;
	MoveToEx(hdc, x, 0, nullptr);
	LineTo(hdc, x, tableHeight);
}

int Table::rowsLeft()
{
	return rows - visibleRows;
}

void Table::handleClick(int clickPosX, int clickPosY) {
	int sortCase = header->getHeaderItemClickedId(clickPosX, clickPosY);
	if (sortCase < 0) return;
	sortCase = (sortCase % 3) + 1;
	sorter->sort(sortCase);

	filesInfoSorted = sorter->getSortedData();
	for (int i = 0;i < rows;i++)
	{
		cells[i][3].text = filesInfoSorted[i].fileName;
		cells[i][4].text = filesInfoSorted[i].fileExtension;
		cells[i][5].text = filesInfoSorted[i].creationDate;
	}
}

int Table::getWidth1()
{
	return (tableWidth - cols - 1) * k1;
}

int Table::getHeaderHeight()
{
	return headerHeight;
}

Table::~Table(){}
