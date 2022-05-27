#pragma once
#define NOMINMAX
#include <windows.h>

class consoleParameters
{
protected:
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND consoleWindow = GetConsoleWindow();
	CONSOLE_CURSOR_INFO structCursorInfo;
	COORD cursorPosition;

public:
	consoleParameters();
	void onCursor();
	void offCursor();
};

consoleParameters::consoleParameters()
{
	GetConsoleCursorInfo(consoleHandle, &structCursorInfo);
	cursorPosition.X = 0;
	cursorPosition.Y = 0;
}

void consoleParameters::onCursor()
{
	structCursorInfo.bVisible = TRUE;
	SetConsoleCursorInfo(consoleHandle, &structCursorInfo);
}

void consoleParameters::offCursor()
{
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &structCursorInfo);
}