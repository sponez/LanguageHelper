#pragma once
#define NOMINMAX
#include <windows.h>

class consoleParameters
{
protected:
	inline static HANDLE consoleHandle;
	inline static HWND consoleWindow;
	inline static RECT consoleWindowCoordinates;
	inline static CONSOLE_CURSOR_INFO structCursorInfo;
	inline static COORD cursorPosition;

public:
	consoleParameters();
	static void onCursor();
	static void offCursor();
	static void GetConsoleCursorPosition();
	static unsigned short getCurrentHeight();
};

consoleParameters::consoleParameters()
{
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	consoleWindow = GetConsoleWindow();
	GetWindowRect(consoleWindow, &consoleWindowCoordinates);
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
		cursorPosition = { 0, 0 };
	}
}

unsigned short consoleParameters::getCurrentHeight()
{
	GetWindowRect(consoleWindow, &consoleWindowCoordinates);
	return (consoleWindowCoordinates.bottom - consoleWindowCoordinates.top) / 16;
}