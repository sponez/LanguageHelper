#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include "consoleParameters.h"
using namespace std;
static const int BACKGROUND_WHITE = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY;
static const int BACKGROUND_BLACK = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

class wconsoleMenu : consoleParameters
{
	wstring selectText{};
	wstring exitText{};
	bool isSelectTextEmpty{};
	bool isExitTextEmpty{};
	vector<pair<wstring, void (*)(wstring&)>> options{};

	void drawMenu(vector<pair<wstring, void (*)(wstring&)>>&, short, wstring); //Draw all options
	void redrawMenu(vector<pair<wstring, void (*)(wstring&)>>&, short, short); //Redraw changed options
	bool selectController(vector<pair<wstring, void (*)(wstring&)>>&, short&, wstring); //Controller for menu
	void displayFilteredOptions(wstring&, vector<pair<wstring, void (*)(wstring&)>>&); //If use select with filter, this is necessary to display the sutable options
	vector<pair<wstring, void (*)(wstring&)>> optionsFilterer(wstring&, vector<short>&); //If use select with filter, this is necessary to filter options by characters
	void setPosition(short&, vector<pair<wstring, void (*)(wstring&)>>&);

public:
	//Initializers
	wconsoleMenu() {}; //Empty console menu
	wconsoleMenu(vector<wstring>&, vector<void (*)(wstring&)>&, wstring, wstring); //Console menu with different functions

//Other public functions
	static void consoleWstringEditor(wstring&, wstring);
	static vector<wstring> wstringSplitter(wstring&, wstring);
	static double DamerauLevenshteinDistance(wstring&, wstring&);

	void singleSelect(short&);
	void singleSelect();
	void singleSelectWithFilter(short&, wstring&, bool);
	void singleSelectWithFilter();
	void cyclicSelect(short&);
	void cyclicSelect();
	void cyclicSelectWithFilter(short&, wstring&, bool);
	void cyclicSelectWithFilter();
};

void wconsoleMenu::consoleWstringEditor(wstring& s, wstring editorText = L"Edit: ")
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorPosition;
	wstring backup = s;

	_wsystem(L"cls");
	cursorPosition.Y = 0;
	cursorPosition.X = editorText.length() + s.length();

	wcout << editorText << s;
	for (;; Sleep(10))
	{
		wchar_t wch;

		if (wch = _getwch())
		{
			if (wch == 27)
			{
				s = backup;
				_wsystem(L"cls");
				break;
			}
			else if (wch == 13)
			{
				_wsystem(L"cls");
				break;
			}
			else if ((char)wch == -32)
			{
				wch = _getwch();

				if (wch == 75)
				{
					if (cursorPosition.X > editorText.length())
					{
						cursorPosition.X--;
						SetConsoleCursorPosition(consoleHandle, cursorPosition);
					}
				}

				if (wch == 77)
				{
					if (cursorPosition.X < (editorText.length() + s.length()))
					{
						cursorPosition.X++;
						SetConsoleCursorPosition(consoleHandle, cursorPosition);
					}
				}
			}
			else
			{
				if (wch == 8)
				{
					if (s.length() > 0 && cursorPosition.X > editorText.length())
					{
						short posToDelete = cursorPosition.X - editorText.length() - 1;
						s.erase(posToDelete, 1);
						cursorPosition.X--;
					}
				}
				else
				{
					short posToInsert = cursorPosition.X - editorText.length();
					s.insert(posToInsert, 1, wch);
					cursorPosition.X++;
				}

				_wsystem(L"cls");
				wcout << editorText << s;
				SetConsoleCursorPosition(consoleHandle, cursorPosition);
			}

		}
	}
}

vector<wstring> wconsoleMenu::wstringSplitter(wstring& s, wstring det = L" ")
{
	vector<wstring> stringParts;
	if (s.empty()) return stringParts;

	for (size_t prevPos = 0, currPos = s.find(det);;)
	{
		size_t len = currPos - prevPos;
		stringParts.push_back(s.substr(prevPos, len));

		if (currPos == wstring::npos)
			break;
		else
		{
			prevPos = currPos + det.length();
			currPos = s.find(det, prevPos);
		}
	}

	return stringParts;
}

double wconsoleMenu::DamerauLevenshteinDistance(wstring& mainWord, wstring& secondaryWord)
{
	double mwl = mainWord.length();
	double swl = secondaryWord.length();
	if (mwl == 0.0 || swl == 0.0)
		return DBL_MAX;

	double deleteCost = 0.9;
	double insertCost = 0.9;
	double replaceCost = 0.7;
	double transposeCost = 0.5;
	double INF = mwl * swl;

	vector<vector<double>> distanceMatrix;
	distanceMatrix.resize(mwl + 1);
	for (int i = 0; i <= mwl; i++)
		distanceMatrix[i].resize(swl + 1);

	distanceMatrix[0][0] = INF;
	for (int i = 0; i < mwl; i++)
	{
		distanceMatrix[i + 1][1] = i * deleteCost;
		distanceMatrix[i + 1][0] = INF;
	}
	for (int j = 0; j < swl; j++)
	{
		distanceMatrix[1][j + 1] = j * insertCost;
		distanceMatrix[0][j + 1] = INF;
	}


	map<wchar_t, int> lastPosition;
	for (int i = 0; i < mwl; i++)
		lastPosition[mainWord[i]] = 0;
	for (int i = 0; i < swl; i++)
		lastPosition[secondaryWord[i]] = 0;

	for (int i = 1; i < mwl; i++)
	{
		double last = 0;
		for (int j = 1; j < swl; j++)
		{
			int _i = lastPosition[secondaryWord[j]];
			int _j = last;
			if (mainWord[i] == secondaryWord[j])
			{
				distanceMatrix[i + 1][j + 1] = distanceMatrix[i][j];
				last = j;
			}
			else
				distanceMatrix[i + 1][j + 1] = min(min((distanceMatrix[i][j] + replaceCost),
					(distanceMatrix[i + 1][j] + insertCost)),
					(distanceMatrix[i][j] + deleteCost));

			distanceMatrix[i + 1][j + 1] = min(distanceMatrix[i + 1][j + 1],
				(distanceMatrix[_i][_j] + (i - _i - 1) * deleteCost + transposeCost + (j - _j - 1) * insertCost));
		}

		lastPosition[mainWord[i]] = i;
	}
	return distanceMatrix[mwl][swl];
}


wconsoleMenu::wconsoleMenu(vector<wstring>& optionNames, vector<void (*)(wstring&)>& optionFunctions, wstring selectText = L"", wstring exitText = L"")
{
	ignore = _setmode(_fileno(stdout), _O_U16TEXT);
	ignore = _setmode(_fileno(stdin), _O_U16TEXT);
	ignore = _setmode(_fileno(stderr), _O_U16TEXT);

	if (optionNames.size() > SHRT_MAX)
	{
		wcout << L"FATAL ERROR. MENU CAN'T BE CREATED. TOO MANY OPTIONS." << endl;
		_wsystem(L"pause");
		exit(-1);
	}

	this->selectText = selectText;
	isSelectTextEmpty = (selectText == L"");

	this->exitText = exitText;
	isExitTextEmpty = (exitText == L"");

	if (optionNames.size() != optionFunctions.size())
	{
		wcout << L"FATAL ERROR. MENU CAN'T BE CREATED. SIZES OF OPTIONS' NAMES ARRAY AND FUNCTIONS' ARRAY ARE DIFFERENT." << endl;
		_wsystem(L"pause");
		exit(-2);
	}

	for (unsigned short i = 0; i < optionNames.size(); i++)
		options.push_back(pair<wstring, void (*)(wstring&)>(optionNames[i], optionFunctions[i]));
}

void wconsoleMenu::drawMenu(vector<pair<wstring, void (*)(wstring&)>>& options, short currentPosition, wstring filterText)
{
	_wsystem(L"cls");
	COORD drawPosition;

	if (!isSelectTextEmpty)
	{
		drawPosition.X = drawPosition.Y = 0;
		wcout << selectText << filterText;
	}

	drawPosition.X = 2;
	for (unsigned short i = 0; i < options.size(); i++)
	{
		drawPosition.Y = (short)!isSelectTextEmpty + i;
		SetConsoleCursorPosition(consoleHandle, drawPosition);

		if (i != currentPosition)
			wcout << options[i].first;
	}

	drawPosition.Y = (short)!isSelectTextEmpty + currentPosition;
	SetConsoleCursorPosition(consoleHandle, drawPosition);
	SetConsoleTextAttribute(consoleHandle, BACKGROUND_WHITE);
	wcout << options[currentPosition].first;
	SetConsoleTextAttribute(consoleHandle, BACKGROUND_BLACK);
}

void wconsoleMenu::redrawMenu(vector<pair<wstring, void (*)(wstring&)>>& options, short currentPosition, short action)
{
	COORD drawPosition;
	short previousPosition = static_cast<short>((options.size() + currentPosition - action) % options.size()); //Find previous position number

	drawPosition.X = 2; //Set space from border
	drawPosition.Y = (short)!isSelectTextEmpty + previousPosition; //Set position of the cursor on previous option
	SetConsoleCursorPosition(consoleHandle, drawPosition);
	wcout << options[previousPosition].first; //Draw previous option with black background

	drawPosition.Y = (short)!isSelectTextEmpty + currentPosition; //Set position of the cursor on current option
	SetConsoleCursorPosition(consoleHandle, drawPosition);

	//Draw previous option with white background
	SetConsoleTextAttribute(consoleHandle, BACKGROUND_WHITE);
	wcout << options[currentPosition].first;
	SetConsoleTextAttribute(consoleHandle, BACKGROUND_BLACK);
}

void wconsoleMenu::displayFilteredOptions(wstring& filterText, vector<pair<wstring, void (*)(wstring&)>>& filteredOptions)
{
	_wsystem(L"cls");
	wcout << selectText << filterText;

	COORD drawPosition;
	drawPosition.X = 2;
	if (filteredOptions.size() > 0)
	{
		for (int i = 0; i < filteredOptions.size(); i++)
		{
			drawPosition.Y = 1 + i;
			SetConsoleCursorPosition(consoleHandle, drawPosition);
			wcout << filteredOptions[i].first;
		}
	}
	else
	{
		drawPosition.Y = 1;
		SetConsoleCursorPosition(consoleHandle, drawPosition);
		wcout << L"Nothing not found! Try something else.";
	}

	SetConsoleCursorPosition(consoleHandle, cursorPosition);
}

bool wconsoleMenu::selectController(vector<pair<wstring, void (*)(wstring&)>>& options, short& currentPosition, wstring filterText = L"")
{
	FlushConsoleInputBuffer(consoleHandle);
	offCursor();

	drawMenu(options, currentPosition, filterText);
	while (GetAsyncKeyState(VK_UP)) { Sleep(10); }
	while (GetAsyncKeyState(VK_DOWN)) { Sleep(10); }
	while (GetAsyncKeyState(VK_RETURN)) { Sleep(10); }
	while (GetAsyncKeyState(VK_BACK)) { Sleep(10); }
	while (GetAsyncKeyState(VK_ESCAPE)) { Sleep(10); }

	for (;; Sleep(10))
	{
		if (IsIconic(consoleWindow)) continue;

		if (GetAsyncKeyState(VK_LBUTTON))
		{
			POINT mousePosition;
			RECT windowRect;
			GetCursorPos(&mousePosition);
			GetWindowRect(consoleWindow, &windowRect);
			if (mousePosition.x < windowRect.left || mousePosition.x > windowRect.right || mousePosition.y < windowRect.top || mousePosition.y > windowRect.bottom)
				ShowWindow(consoleWindow, SW_MINIMIZE);

			while (GetAsyncKeyState(VK_LBUTTON)) { Sleep(10); }
		}

		if (GetAsyncKeyState(VK_MENU) && GetAsyncKeyState(VK_TAB))
			ShowWindow(consoleWindow, SW_MINIMIZE);

		for (int i = 0; GetAsyncKeyState(VK_UP); Sleep(200 - 10 * i))
		{
			currentPosition = static_cast<short>((options.size() + currentPosition - 1) % options.size());
			redrawMenu(options, currentPosition, -1);
			if (i < 19) i++;
		}

		for (int i = 0; GetAsyncKeyState(VK_DOWN); Sleep(200 - 10 * i))
		{
			currentPosition = (currentPosition + 1) % options.size();
			redrawMenu(options, currentPosition, 1);
			if (i < 19) i++;
		}

		if (GetAsyncKeyState(VK_RETURN))
		{
			wcin.ignore(LLONG_MAX, '\n');
			while (_kbhit()) ignore = _getwch();

			onCursor();
			_wsystem(L"cls");

			return true;
		}

		if (GetAsyncKeyState(VK_BACK) || GetAsyncKeyState(VK_ESCAPE))
		{
			while (_kbhit()) ignore = _getwch();

			currentPosition = 0;
			onCursor();
			_wsystem(L"cls");

			return false;
		}
	}
}

void wconsoleMenu::setPosition(short& position, vector<pair<wstring, void (*)(wstring&)>>& options)
{
	if (position >= options.size())
		position = options.size() - 1;
	else if (position < 0)
		position = 0;
}

void wconsoleMenu::singleSelect(short& position)
{
	if (options.size() == 0)
	{
		position = -1;
		return;
	}

	setPosition(position, options);

	if (!isExitTextEmpty)
		options.push_back(pair<wstring, void (*)(wstring&)>(exitText, [](wstring&) {}));

	if (selectController(options, position))
	{
		options[position].second(options[position].first);

		if (!isExitTextEmpty)
		{
			options.pop_back();
			if (position == options.size())
				position = -1;
			return;
		}
		else
			return;
	}
	else
	{
		position = -1;
		return;
	}
}

void wconsoleMenu::singleSelect()
{
	short position = 0;
	singleSelect(position);
}

void wconsoleMenu::cyclicSelect(short& position)
{
	if (options.size() == 0)
	{
		position = -1;
		return;
	}

	setPosition(position, options);

	if (!isExitTextEmpty)
		options.push_back(pair<wstring, void (*)(wstring&)>(exitText, [](wstring&) {}));

	do
	{
		if (selectController(options, position))
			options[position].second(options[position].first);
		else
		{
			position = -1;
			return;
		}
	} while (isExitTextEmpty || position != options.size() - 1);

	if (!isExitTextEmpty) options.pop_back();
	position = -1;
	return;
}

void wconsoleMenu::cyclicSelect()
{
	short position = 0;
	cyclicSelect(position);
}

vector<pair<wstring, void (*)(wstring&)>> wconsoleMenu::optionsFilterer(wstring& filterText, vector<short>& listOfRealOptionsNumbers)
{
	_wsystem(L"cls");

	vector<pair<wstring, void (*)(wstring&)>> filteredOptions;
	listOfRealOptionsNumbers.clear();

	for (short i = 0; i < options.size(); i++)
	{
		vector<wstring> wordsInFilterText = wstringSplitter(filterText);
		vector<wstring> wordsInOptionName = wstringSplitter(options[i].first);
		short matchAmount = 0;

		for (int k = 0; k < wordsInFilterText.size(); k++)
		{
			for (int _i = 0; _i < wordsInOptionName.size(); _i++)
			{
				bool isSuitable = false;

				for (int _j = 0; (_j + wordsInFilterText[k].length() <= wordsInOptionName[_i].length()); _j++)
				{
					wstring subWstring = wordsInOptionName[_i].substr(_j, wordsInFilterText[k].length());
					if (DamerauLevenshteinDistance(wordsInFilterText[k], subWstring) <= (int)(wordsInFilterText[k].length() / 5))
					{
						isSuitable = true;
						matchAmount++;
						wordsInOptionName.erase(wordsInOptionName.begin() + _i);
						break;
					}
				}

				if (isSuitable) break;
			}
		}

		if (matchAmount == wordsInFilterText.size())
		{
			filteredOptions.push_back(options[i]);
			listOfRealOptionsNumbers.push_back(i);
		}
	}
	if (!isExitTextEmpty) filteredOptions.push_back(pair<wstring, void (*)(wstring&)>(exitText, [](wstring&) {}));

	return filteredOptions;
}

void wconsoleMenu::singleSelectWithFilter(short& position, wstring& filterText, bool skipFilter = true)
{
	cursorPosition.X = selectText.length() + filterText.length();

	if (options.size() == 0)
	{
		position = -1;
		return;
	}
	setPosition(position, options);

	if (isSelectTextEmpty) isSelectTextEmpty = false;
	vector<short> listOfRealOptionsNumbers;
	vector<pair<wstring, void (*)(wstring&)>> filteredOptions = optionsFilterer(filterText, listOfRealOptionsNumbers);

	displayFilteredOptions(filterText, filteredOptions);
	if (skipFilter && filteredOptions.size() != 0)
	{
		short positionInFilteredList = 0;
		for (int i = 0; i < listOfRealOptionsNumbers.size(); i++)
		{
			if (position == listOfRealOptionsNumbers[i])
				positionInFilteredList = i;
		}

		if (selectController(filteredOptions, positionInFilteredList, filterText))
		{
			if (!isExitTextEmpty && positionInFilteredList == filteredOptions.size() - 1)
				position = -1;
			else
				position = listOfRealOptionsNumbers[positionInFilteredList];

			filteredOptions[positionInFilteredList].second(filteredOptions[positionInFilteredList].first);

			if (!isExitTextEmpty)
				filteredOptions.pop_back();

			return;
		}
		else
		{
			position = listOfRealOptionsNumbers[0];
			displayFilteredOptions(filterText, filteredOptions);
		}
	}

	for (;; Sleep(10))
	{
		wchar_t wch;

		if (wch = _getwch())
		{
			if (wch == 27)
			{
				_wsystem(L"cls");
				isSelectTextEmpty = (selectText == L"");
				position = -1;
				return;
			}
			else if (wch == 13)
			{
				if (filteredOptions.size() == 0) continue;

				short positionInFilteredList = 0;
				for (int i = 0; i < listOfRealOptionsNumbers.size(); i++)
				{
					if (position == listOfRealOptionsNumbers[i])
						positionInFilteredList = i;
				}

				if (selectController(filteredOptions, positionInFilteredList, filterText))
				{
					if (!isExitTextEmpty && positionInFilteredList == filteredOptions.size() - 1)
						position = -1;
					else
						position = listOfRealOptionsNumbers[positionInFilteredList];

					filteredOptions[positionInFilteredList].second(filteredOptions[positionInFilteredList].first);

					if (!isExitTextEmpty)
						filteredOptions.pop_back();

					return;
				}
				else
				{
					position = listOfRealOptionsNumbers[0];
					displayFilteredOptions(filterText, filteredOptions);
					continue;
				}
			}
			else
			{
				if ((char)wch == -32)
				{
					wch = _getwch();

					if (wch == 75)
					{
						if (cursorPosition.X > selectText.length())
						{
							cursorPosition.X--;
							SetConsoleCursorPosition(consoleHandle, cursorPosition);
						}
					}

					if (wch == 77)
					{
						if (cursorPosition.X < (selectText.length() + filterText.length()))
						{
							cursorPosition.X++;
							SetConsoleCursorPosition(consoleHandle, cursorPosition);
						}
					}
				}
				else
				{
					if (wch == 8)
					{
						if (filterText.length() > 0 && cursorPosition.X > selectText.length())
						{
							short posToDelete = cursorPosition.X - selectText.length() - 1;
							filterText.erase(posToDelete, 1);
							cursorPosition.X--;
						}
					}
					else
					{
						short posToInsert = cursorPosition.X - selectText.length();
						filterText.insert(posToInsert, 1, wch);
						cursorPosition.X++;
					}

					filteredOptions = optionsFilterer(filterText, listOfRealOptionsNumbers);
					displayFilteredOptions(filterText, filteredOptions);
				}
			}
		}
	}
}

void wconsoleMenu::singleSelectWithFilter()
{
	wstring filterText = L"";
	short position = 0;
	singleSelectWithFilter(position, filterText, false);
}

void wconsoleMenu::cyclicSelectWithFilter(short& position, wstring& filterText, bool skipFilter = true)
{
	cursorPosition.X = selectText.length() + filterText.length();

	if (options.size() == 0)
	{
		position = -1;
		return;
	}
	if (isSelectTextEmpty) isSelectTextEmpty = false;
	setPosition(position, options);

	vector<short> listOfRealOptionsNumbers;
	vector<pair<wstring, void (*)(wstring&)>> filteredOptions = optionsFilterer(filterText, listOfRealOptionsNumbers);

	displayFilteredOptions(filterText, filteredOptions);

	if (skipFilter && filteredOptions.size() != 0)
	{
		short positionInFilteredList = 0;
		for (int i = 0; i < listOfRealOptionsNumbers.size(); i++)
		{
			if (position == listOfRealOptionsNumbers[i])
				positionInFilteredList = i;
		}

		do
		{
			if (selectController(filteredOptions, positionInFilteredList, filterText))
			{
				if (!isExitTextEmpty && positionInFilteredList == filteredOptions.size() - 1)
					position = -1;
				else
					position = listOfRealOptionsNumbers[positionInFilteredList];

				filteredOptions[positionInFilteredList].second(filteredOptions[positionInFilteredList].first);
			}
			else
				break;
		} while (isExitTextEmpty || positionInFilteredList != filteredOptions.size() - 1);

		position = listOfRealOptionsNumbers[0];
		displayFilteredOptions(filterText, filteredOptions);
	}

	for (;; Sleep(10))
	{
		wchar_t wch;

		if (wch = _getwch())
		{
			if (wch == 27)
			{
				_wsystem(L"cls");
				isSelectTextEmpty = (selectText == L"");
				position = -1;
				return;
			}
			else if (wch == 13)
			{
				if (filteredOptions.size() == 0) continue;

				short positionInFilteredList = 0;
				for (int i = 0; i < listOfRealOptionsNumbers.size(); i++)
				{
					if (position == listOfRealOptionsNumbers[i])
						positionInFilteredList = i;
				}

				do
				{
					if (selectController(filteredOptions, positionInFilteredList, filterText))
					{
						if (!isExitTextEmpty && positionInFilteredList == filteredOptions.size() - 1)
							position = -1;
						else
							position = listOfRealOptionsNumbers[positionInFilteredList];

						filteredOptions[positionInFilteredList].second(filteredOptions[positionInFilteredList].first);
					}
					else
						break;
				} while (isExitTextEmpty || positionInFilteredList != filteredOptions.size() - 1);

				position = listOfRealOptionsNumbers[0];
				displayFilteredOptions(filterText, filteredOptions);
			}
			else
			{
				if ((char)wch == -32)
				{
					wch = _getwch();

					if (wch == 75)
					{
						if (cursorPosition.X > selectText.length())
						{
							cursorPosition.X--;
							SetConsoleCursorPosition(consoleHandle, cursorPosition);
						}
					}

					if (wch == 77)
					{
						if (cursorPosition.X < (selectText.length() + filterText.length()))
						{
							cursorPosition.X++;
							SetConsoleCursorPosition(consoleHandle, cursorPosition);
						}
					}
				}
				else
				{
					if (wch == 8)
					{
						if (filterText.length() > 0 && cursorPosition.X > selectText.length())
						{
							short posToDelete = cursorPosition.X - selectText.length() - 1;
							filterText.erase(posToDelete, 1);
							cursorPosition.X--;
						}
					}
					else
					{
						short posToInsert = cursorPosition.X - selectText.length();
						filterText.insert(posToInsert, 1, wch);
						cursorPosition.X++;
					}

					filteredOptions = optionsFilterer(filterText, listOfRealOptionsNumbers);
					displayFilteredOptions(filterText, filteredOptions);
				}
			}
		}
	}
}

void wconsoleMenu::cyclicSelectWithFilter()
{
	wstring filterText = L"";
	short position = 0;
	cyclicSelectWithFilter(position, filterText, false);
}