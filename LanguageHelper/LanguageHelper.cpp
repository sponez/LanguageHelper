#pragma warning(disable : 4996)
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <codecvt>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include <time.h>
#include <cmath>
#include "ConsoleMenu.h"

using namespace std;
wstring globalPath;
wstring firstLanguage;
wstring secondLanguage;

void createDirrectory(wstring path)
{
	struct _stat buf;
	if (_wstat(&path[0], &buf) != 0)
	{
		wstring command = L"md " + path;
		_wsystem(&command[0]);
	}
}

void wstringToLower(wstring& s)
{
	for (int i = 0; i < s.length(); i++)
		s[i] = tolower(s[i], locale(""));
}

wstring wstringStandartForm(wstring s)
{
	wstringToLower(s);
	s[0] = toupper(s[0], locale(""));
	return s;
}

double findMaxRate(vector<vector<bool>>& matrix, int curri, int currj)
{
	int sa = matrix.size(), sb = matrix[0].size();

	double maxRate = 0.0, rate = 0.0;
	int i = curri;
	for (int j = currj; j < sb;)
	{
		if (i < sa)
		{
			if (matrix[i][j])
			{
				rate += 1.0;
				i++;
				j++;
			}
			else
			{
				if (i < sa - 1)
				{
					double subRate = rate + findMaxRate(matrix, i + 1, j);
					maxRate = max(maxRate, subRate);
				}

				rate -= 0.4;
				j++;
			}
		}
		else
		{
			rate -= 0.4;
			j++;
		}
	}

	return max(maxRate, rate);
}

double matchWords(wstring mainWord, wstring secondaryWord)
{
	if (mainWord.length() == 0 || secondaryWord.length() == 0 || abs(static_cast<int>(mainWord.length() - secondaryWord.length())) >= 3)
		return 0.0;

	vector<vector<bool>> matchMatrix;
	int amountOfColoumns = max(mainWord.length(), secondaryWord.length());
	for (int i = 0; i < mainWord.length(); i++)
	{
		vector<bool> iLine;
		iLine.resize(amountOfColoumns, false);

		for (int j = 0; j < secondaryWord.length(); j++)
			iLine[j] = (mainWord[i] == secondaryWord[j]);
		
		matchMatrix.push_back(iLine);
	}

	return findMaxRate(matchMatrix, 0, 0) / (double)mainWord.length();
}

wstring randomWordFrom(wstring path)
{
	vector<wstring> words;
	for (auto const& fileIterator : filesystem::directory_iterator{ path })
	{
		wstring word = fileIterator.path().filename();
		words.push_back(word);
	}

	int amountOfWords = words.size();
	if (amountOfWords == 0)
	{
		wcout << "Add some words at first" << endl;
		_wsystem(L"pause");
		return L"";
	}
	else
	{
		int randomIndex = rand() % amountOfWords;
		return words[randomIndex];
	}
}

void firstLanguageToSecondLanguageTest()
{
	wstring firstLanguageDirrectory = globalPath + L"\\" + wstringStandartForm(firstLanguage) + L"Words";
	createDirrectory(firstLanguageDirrectory);
	
	int amountOfRepeats;
	wcout << L"Enter amount of test repetitions: ";
	wcin >> amountOfRepeats;
	wcin.ignore(LLONG_MAX, '\n');

	int amountOfCorrectAnswers = 0;
	for (int i = 0; i < amountOfRepeats; i++)
	{
		_wsystem(L"cls");

		wstring word = randomWordFrom(firstLanguageDirrectory);
		if (word == L"") return;

		wifstream wordFile(firstLanguageDirrectory + L"\\" + word);
		wordFile.imbue(std::locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wcout << word << L" is: ";
		
		wstring answer;
		getline(wcin, answer);
		wstringToLower(answer);

		double maxMatchRate = 0.0;
		vector<wstring> secondLanguageTranslates;
		for (wstring secondLanguageWord; getline(wordFile, secondLanguageWord);)
		{
			secondLanguageTranslates.push_back(secondLanguageWord);
			maxMatchRate = max(maxMatchRate, matchWords(secondLanguageWord, answer));
		}
		wordFile.close();

		if (maxMatchRate == 1.0)
		{
			wcout << L"Absolutely correct!" << endl;

			if (secondLanguageTranslates.size() == 1)
			{
				wcout << L"Right is " << secondLanguageTranslates[0] << endl;
			}
			else
			{
				wcout << L"Right is one of:" << endl;
				for (int j = 0; j < secondLanguageTranslates.size(); j++)
					wcout << secondLanguageTranslates[j] << endl;
			}

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else if (maxMatchRate > 0.6)
		{
			wcout << L"Almost correct." << endl;

			if (secondLanguageTranslates.size() == 1)
			{
				wcout << L"Correct would be " << secondLanguageTranslates[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < secondLanguageTranslates.size(); j++)
					wcout << secondLanguageTranslates[j] << endl;
			}

			wcout << L"But it will be scored!" << endl;

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else
		{
			wcout << L"Nope." << endl;
			
			if (secondLanguageTranslates.size() == 1)
			{
				wcout << L"Correct would be " << secondLanguageTranslates[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < secondLanguageTranslates.size(); j++)
					wcout << secondLanguageTranslates[j] << endl;
			}

			wcout << L"It won't be scored!" << endl;

			_wsystem(L"pause");
		}
	}

	_wsystem(L"cls");
	wcout << L"You was right in " << amountOfCorrectAnswers * 100 / amountOfRepeats << L"% of cases" << endl;
	_wsystem(L"pause");
}

void secondLanguageToFirstLanguageTest()
{
	wstring secondLanguageDirrectory = globalPath + L"\\" + wstringStandartForm(secondLanguage) + L"Words";
	createDirrectory(secondLanguageDirrectory);

	int amountOfRepeats;
	wcout << L"Enter amount of test repetitions: ";
	wcin >> amountOfRepeats;
	wcin.ignore(LLONG_MAX, '\n');

	int amountOfCorrectAnswers = 0;
	for (int i = 0; i < amountOfRepeats; i++)
	{
		_wsystem(L"cls");

		wstring word = randomWordFrom(secondLanguageDirrectory);
		if (word == L"") return;

		wifstream wordFile(secondLanguageDirrectory + L"\\" + word);
		wordFile.imbue(std::locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wcout << word << L" is: ";

		wstring answer;
		getline(wcin, answer);
		wstringToLower(answer);

		double maxMatchRate = 0.0;
		vector<wstring> firstLanguageTranslates;
		for (wstring firstLanguageWord; getline(wordFile, firstLanguageWord);)
		{
			firstLanguageTranslates.push_back(firstLanguageWord);
			maxMatchRate = max(maxMatchRate, matchWords(firstLanguageWord, answer));
		}
		wordFile.close();

		if (maxMatchRate == 1.0)
		{
			wcout << L"Absolutely correct!" << endl;

			if (firstLanguageTranslates.size() == 1)
			{
				wcout << L"Right is: " << firstLanguageTranslates[0] << endl;
			}
			else
			{
				wcout << L"Right is one of:" << endl;
				for (int j = 0; j < firstLanguageTranslates.size(); j++)
					wcout << firstLanguageTranslates[j] << endl;
			}

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else if (maxMatchRate > 0.6)
		{
			wcout << L"Almost correct." << endl;

			if (firstLanguageTranslates.size() == 1)
			{
				wcout << L"Correct would be " << firstLanguageTranslates[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < firstLanguageTranslates.size(); j++)
					wcout << firstLanguageTranslates[j] << endl;
			}

			wcout << L"But it will be scored!" << endl;

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else
		{
			wcout << L"Nope." << endl;

			if (firstLanguageTranslates.size() == 1)
			{
				wcout << L"Correct would be " << firstLanguageTranslates[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < firstLanguageTranslates.size(); j++)
					wcout << firstLanguageTranslates[j] << endl;
			}

			wcout << L"It won't be scored!" << endl;

			_wsystem(L"pause");
		}
	}

	_wsystem(L"cls");
	wcout << L"You was right in " << amountOfCorrectAnswers * 100 / amountOfRepeats << L"% of cases" << endl;
	_wsystem(L"pause");
}

void addFirstLanguageWord()
{
	wstring firstLanguageDirrectory = globalPath + L"\\" + firstLanguage + L"Words";
	createDirrectory(firstLanguageDirrectory);

	wstring firstLanguageWord;
	wcout << L"Enter a " << firstLanguage << L" word: ";
	getline(wcin, firstLanguageWord);
	wstringToLower(firstLanguageWord);

	if (firstLanguageWord.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}
	else
	{
		wofstream wordFile(firstLanguageDirrectory + L"\\" + firstLanguageWord);
		wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wstring secondLanguageTranslate = L" ";
		vector<wstring> secondLanguageTranslates;
		while (secondLanguageTranslate.length() > 0)
		{
			wcout << L"Enter a " << secondLanguage << L" translate (if you want to exit just press enter): ";
			getline(wcin, secondLanguageTranslate);
			wstringToLower(secondLanguageTranslate);

			if (secondLanguageTranslate.length() > 0)
			{
				secondLanguageTranslates.push_back(secondLanguageTranslate);
				wordFile << secondLanguageTranslate << endl;
			}
		}
		wordFile.close();

		if (secondLanguageTranslates.size() == 0)
		{
			wcout << "No one translates haven't entered." << endl;
			_wsystem(L"pause");

			wstring command = L"del " + firstLanguageDirrectory + L"\\" + firstLanguageWord;
			_wsystem(&command[0]);
			return;
		}
	}
}

void addSecondLanguagehWord()
{
	wstring secondLanguageDirrectory = globalPath + L"\\" + secondLanguage + L"Words";
	createDirrectory(secondLanguageDirrectory);

	wstring secondLanguageWord;
	wcout << L"Enter a " << secondLanguage << L" word: ";
	getline(wcin, secondLanguageWord);
	wstringToLower(secondLanguageWord);

	if (secondLanguageWord.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}
	else
	{
		wofstream wordFile(secondLanguageDirrectory + L"\\" + secondLanguageWord);
		wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wstring firstLanguageTranslate = L" ";
		vector<wstring> firstLanguageTranslates;
		while (firstLanguageTranslate.length() > 0)
		{
			wcout << L"Enter a " << firstLanguage << L" translate (if you want to exit just press enter): ";
			getline(wcin, firstLanguageTranslate);
			wstringToLower(firstLanguageTranslate);

			if (firstLanguageTranslate.length() > 0)
			{
				firstLanguageTranslates.push_back(firstLanguageTranslate);
				wordFile << firstLanguageTranslate << endl;
			}
		}
		wordFile.close();

		if (firstLanguageTranslates.size() == 0)
		{
			wcout << "No one translates haven't entered." << endl;
			_wsystem(L"pause");

			wstring command = L"del " + secondLanguageDirrectory + L"\\" + secondLanguageWord;
			_wsystem(&command[0]);
			return;
		}
	}
}

void addWordFunction()
{

	vector<wstring> languages = { wstringStandartForm(firstLanguage), wstringStandartForm(secondLanguage) };
	vector<void (*)()> addWordFunctions = { addFirstLanguageWord, addSecondLanguagehWord };
	consoleMenu languageOfWord(languages, addWordFunctions);
	languageOfWord.select();
}

void testingFunction()
{
	vector<wstring> testingTypes = { wstringStandartForm(firstLanguage) + L" to " + secondLanguage , wstringStandartForm(secondLanguage) + L" to " + firstLanguage };
	vector<void (*)()> testingFunctions = { firstLanguageToSecondLanguageTest, secondLanguageToFirstLanguageTest };
	consoleMenu testingTypeMenu(testingTypes, testingFunctions);
	testingTypeMenu.select();
}

int main(int argc, wchar_t* argv[])
{
	int setmode;
	setmode = _setmode(_fileno(stdout), _O_U16TEXT);
	setmode = _setmode(_fileno(stdin), _O_U16TEXT);
	setmode = _setmode(_fileno(stderr), _O_U16TEXT);

	wchar_t c_username[257];
	DWORD username_len = 257;
	GetUserNameW(c_username, &username_len);
	globalPath = L"C:\\Users\\" + wstring(c_username) + L"\\AppData\\Local\\LanguageHelper";
	createDirrectory(globalPath);

	wifstream savedLanguagesFile(globalPath + L"\\languages.cfg");
	if (savedLanguagesFile.is_open())
	{
		savedLanguagesFile >> firstLanguage >> secondLanguage;
		savedLanguagesFile.close();
	}
	else
	{
		wcout << L"Global initializing.\nPlease, enter the first using language: ";
		wcin >> firstLanguage;
		wstringToLower(firstLanguage);

		wcout << L"Please, enter the second using language: ";
		wcin >> secondLanguage;
		wstringToLower(secondLanguage);

		wofstream languages(globalPath + L"\\languages.cfg");
		languages << firstLanguage << endl << secondLanguage;
		languages.close();
	}

	vector<wstring> options = { L"Testing" , L"Add word" };
	vector<void (*)()> functions = { testingFunction, addWordFunction };
	consoleMenu languageHelperMenu(options, functions, true);

	srand(time(NULL));
	languageHelperMenu.select();
}