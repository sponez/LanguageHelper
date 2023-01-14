#pragma once
#define NOMINMAX
#include <windows.h>

class consoleParameters
{
protected:
	inline static HANDLE consoleHandle;
	inline static HWND consoleWindow;
	inline static CONSOLE_CURSOR_INFO structCursorInfo;
	inline static COORD cursorPosition;

public:
	consoleParameters();
	static void onCursor();
	static void offCursor();
	static void GetConsoleCursorPosition();
};

consoleParameters::consoleParameters()
{
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	consoleWindow = GetConsoleWindow();
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

void consoleParameters::GetConsoleCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(consoleHandle, &cbsi))
	{
		cursorPosition = cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		cursorPosition ={ 0, 0 };
	}
}