#include "Cell.h"


Cell::Cell(std::wstring text) {
	this->text = text;
}

void Cell::draw(HDC hdc) {
		
	RECT rect = { x, y, x + width, y + height };
	SIZE textSize;
	std::wstring buffer(100, L'\0');
	int dif, letterSize;

	GetTextExtentPoint32(hdc, text.c_str(), text.length(), &textSize);
	letterSize = textSize.cx / text.length();
	dif = textSize.cx - width;
	if (dif > 0) buffer = text.substr(0, text.length() - dif / letterSize - 4) + L"...";
	else buffer = text;
	TextOut(hdc, x + 1, y, buffer.c_str(), buffer.size());	
}

Cell::~Cell(){}