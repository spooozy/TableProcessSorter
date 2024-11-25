#include "Header.h"


Header::Header(int tableColsNum) {
	this->tableColsNum = tableColsNum;
	headerItems.resize(tableColsNum);
};

void Header::addHeaderItem(std::wstring text, int id, int x, int width) {
	headerItems[id].x = x;
	headerItems[id].text = text;
	headerItems[id].width = width;
}

void Header::drawHeader(HDC hdc) {
	SIZE textSize;
	std::wstring buffer(100, L'\0');
	int hx, hy, dif, letterSize, diff;

	for (int i = 0;i < tableColsNum;i++)
	{
		GetTextExtentPoint32(hdc, headerItems[i].text.c_str(), headerItems[i].text.length(), &textSize);
		letterSize = textSize.cx / headerItems[i].text.length();
		dif = textSize.cx - headerItems[i].width;
		if (dif > 0) buffer = headerItems[i].text.substr(0, headerItems[i].text.length() - dif / letterSize - 3) + L"...";
		else buffer = headerItems[i].text.c_str();
		GetTextExtentPoint32(hdc, buffer.c_str(), buffer.length(), &textSize);
		hx = (headerItems[i].x + headerItems[i].width/2) - (textSize.cx / 2);
		hy = ((height - 1) / 2) - (textSize.cy / 2);
		TextOut(hdc, hx, hy, buffer.c_str(), buffer.length());
	}	
}

int Header::getHeaderItemClickedId(int x, int y) {
	if (y > height) return -1;
	for (int i = 0;i < tableColsNum;i++) {
		if (x >= headerItems[i].x && x <= headerItems[i].width + headerItems[i].x) return i;
	}
	return -1;
}

Header::~Header() {
	headerItems.clear();
}