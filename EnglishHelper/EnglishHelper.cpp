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
#include "ConsoleMenu.h"

using namespace std;
wstring username;

void createDirrectory(wstring path)
{
	struct _stat buf;
	if (_wstat(&path[0], &buf) != 0)
	{
		wstring command = L"md " + path;
		_wsystem(&command[0]);
	}
}

void wstringToLowerEng(wstring& s)
{
	for (int i = 0; i < s.length(); i++)
		s[i] = tolower(s[i]);
}

void wstringToLowerRus(wstring& s)
{
	for (int i = 0; i < s.length(); i++)
		s[i] = tolower(s[i], locale(""));
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
	if (mainWord.length() == 0 || secondaryWord.length() == 0 || mainWord.length() / secondaryWord.length() >= 3 || secondaryWord.length() / mainWord.length() >= 3)
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

void englishToRussianTest()
{
	wstring path = L"C:\\Users\\" + username + L"\\AppData\\Local\\EnglishHelper\\EnglishWords";
	createDirrectory(path);
	
	int amountOfRepeats;
	wcout << L"Enter amount of test repetitions: ";
	wcin >> amountOfRepeats;
	wcin.ignore(LLONG_MAX, '\n');

	int amountOfCorrectAnswers = 0;
	for (int i = 0; i < amountOfRepeats; i++)
	{
		_wsystem(L"cls");

		wstring word = randomWordFrom(path);
		if (word == L"") return;

		wifstream wordFile(path + L"\\" + word);
		wordFile.imbue(std::locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wcout << word << L" is: ";
		
		wstring answer;
		getline(wcin, answer);
		wstringToLowerRus(answer);

		double maxMatchRate = 0.0;
		vector<wstring> russianTranslates;
		for (wstring russianWord; getline(wordFile, russianWord);)
		{
			russianTranslates.push_back(russianWord);
			maxMatchRate = max(maxMatchRate, matchWords(russianWord, answer));
		}
		wordFile.close();

		if (maxMatchRate == 1.0)
		{
			wcout << L"Absolutely correct!" << endl;

			if (russianTranslates.size() == 1)
			{
				wcout << L"Right is: " << russianTranslates[0] << endl;
			}
			else
			{
				wcout << L"Right is one of:" << endl;
				for (int j = 0; j < russianTranslates.size(); j++)
					wcout << russianTranslates[j] << endl;
			}

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else if (maxMatchRate > 0.6)
		{
			wcout << L"Almost correct." << endl;

			if (russianTranslates.size() == 1)
			{
				wcout << L"Correct would be " << russianTranslates[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < russianTranslates.size(); j++)
					wcout << russianTranslates[j] << endl;
			}

			wcout << L"But it will be scored!" << endl;

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else
		{
			wcout << L"Nope." << endl;
			
			if (russianTranslates.size() == 1)
			{
				wcout << L"Correct would be " << russianTranslates[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < russianTranslates.size(); j++)
					wcout << russianTranslates[j] << endl;
			}

			wcout << L"It won't be scored!" << endl;

			_wsystem(L"pause");
		}
	}

	_wsystem(L"cls");
	wcout << L"You was right in " << amountOfCorrectAnswers * 100 / amountOfRepeats << L"% of cases" << endl;
	_wsystem(L"pause");
}

void russianToEnglishTest()
{
	wstring path = L"C:\\Users\\" + username + L"\\AppData\\Local\\EnglishHelper\\RussianWords";
	createDirrectory(path);

	int amountOfRepeats;
	wcout << L"Enter amount of test repetitions: ";
	wcin >> amountOfRepeats;
	wcin.ignore(LLONG_MAX, '\n');

	int amountOfCorrectAnswers = 0;
	for (int i = 0; i < amountOfRepeats; i++)
	{
		_wsystem(L"cls");

		wstring word = randomWordFrom(path);
		if (word == L"") return;

		wifstream wordFile(path + L"\\" + word);
		wordFile.imbue(std::locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wcout << word << L" is: ";

		wstring answer;
		getline(wcin, answer);
		wstringToLowerRus(answer);

		double maxMatchRate = 0.0;
		vector<wstring> englishTranslates;
		for (wstring englishWord; getline(wordFile, englishWord);)
		{
			englishTranslates.push_back(englishWord);
			maxMatchRate = max(maxMatchRate, matchWords(englishWord, answer));
		}
		wordFile.close();

		if (maxMatchRate == 1.0)
		{
			wcout << L"Absolutely correct!" << endl;

			if (englishTranslates.size() == 1)
			{
				wcout << L"Right is: " << englishTranslates[0] << endl;
			}
			else
			{
				wcout << L"Right is one of:" << endl;
				for (int j = 0; j < englishTranslates.size(); j++)
					wcout << englishTranslates[j] << endl;
			}

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else if (maxMatchRate > 0.6)
		{
			wcout << L"Almost correct." << endl;

			if (englishTranslates.size() == 1)
			{
				wcout << L"Correct would be " << englishTranslates[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < englishTranslates.size(); j++)
					wcout << englishTranslates[j] << endl;
			}

			wcout << L"But it will be scored!" << endl;

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else
		{
			wcout << L"Nope." << endl;

			if (englishTranslates.size() == 1)
			{
				wcout << L"Correct would be " << englishTranslates[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < englishTranslates.size(); j++)
					wcout << englishTranslates[j] << endl;
			}

			wcout << L"It won't be scored!" << endl;

			_wsystem(L"pause");
		}
	}

	_wsystem(L"cls");
	wcout << L"You was right in " << amountOfCorrectAnswers * 100 / amountOfRepeats << L"% of cases" << endl;
	_wsystem(L"pause");
}

void addEnglishWord()
{
	wstring path = L"C:\\Users\\" + username + L"\\AppData\\Local\\EnglishHelper\\EnglishWords";
	createDirrectory(path);

	wstring englishWord;
	wcout << L"Enter an english word: ";
	getline(wcin, englishWord);
	wstringToLowerEng(englishWord);

	if (englishWord.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}
	else
	{
		wofstream wordFile(path + L"\\" + englishWord);
		wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wstring russianTranslate = L" ";
		vector<wstring> russianTranslates;
		while (russianTranslate.length() > 0)
		{
			wcout << L"Enter a russian translate (if you want to exit just press enter): ";
			getline(wcin, russianTranslate);
			wstringToLowerRus(russianTranslate);

			if (russianTranslate.length() > 0)
			{
				russianTranslates.push_back(russianTranslate);
				wordFile << russianTranslate << endl;
			}
		}
		wordFile.close();

		if (russianTranslates.size() == 0)
		{
			wcout << "No one translates haven't entered." << endl;
			_wsystem(L"pause");

			wstring command = L"del " + path + L"\\" + englishWord;
			_wsystem(&command[0]);
			return;
		}
	}
}

void addRussianhWord()
{
	wstring path = L"C:\\Users\\" + username + L"\\AppData\\Local\\EnglishHelper\\RussianWords";
	createDirrectory(path);

	wstring russianWord;
	wcout << L"Enter a russian word: ";
	getline(wcin, russianWord);
	wstringToLowerRus(russianWord);

	if (russianWord.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}
	else
	{
		wofstream wordFile(path + L"\\" + russianWord);
		wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wstring englishTranslate = L" ";
		vector<wstring> englishTranslates;
		while (englishTranslate.length() > 0)
		{
			wcout << L"Enter an english translate (if you want to exit just press enter): ";
			getline(wcin, englishTranslate);
			wstringToLowerEng(englishTranslate);

			if (englishTranslate.length() > 0)
			{
				englishTranslates.push_back(englishTranslate);
				wordFile << englishTranslate << endl;
			}
		}
		wordFile.close();

		if (englishTranslates.size() == 0)
		{
			wcout << "No one translates haven't entered." << endl;
			_wsystem(L"pause");

			wstring command = L"del " + path + L"\\" + russianWord;
			_wsystem(&command[0]);
			return;
		}
	}
}

void addWordFunction()
{
	vector<wstring> languages = { L"English", L"Russian" };
	vector<void (*)()> addWordFunctions = { addEnglishWord, addRussianhWord };
	consoleMenu languageOfWord(languages, addWordFunctions);
	languageOfWord.select();
}

void testingFunction()
{
	vector<wstring> testingTypes = { L"English to russian", L"Russian to english" };
	vector<void (*)()> testingFunctions = { englishToRussianTest, russianToEnglishTest };
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
	username = wstring(c_username);
	createDirrectory(L"C:\\Users\\" + username + L"\\AppData\\Local\\EnglishHelper");

	vector<wstring> options = { L"Testing" , L"Add word" };
	vector<void (*)()> functions = { testingFunction, addWordFunction };
	consoleMenu englishHelperMenu(options, functions, true);

	srand(time(NULL));
	englishHelperMenu.select();
}