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
#include <cmath>
#include <random>
#include "ConsoleMenu.h"

using namespace std;
wstring globalPath;
wstring firstLanguage;
wstring secondLanguage;
mt19937 radnomNumber(time(0));

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

void removeTextInBracket(wstring& string)
{
	int i = 0;

	while (i < string.length())
	{
		int leftBracket = -1;
		for (; i < string.length(); i++)
		{
			if (string[i] == L'(' && leftBracket == -1)
				leftBracket = i;

			if (string[i] == L')' && leftBracket > -1)
			{
				int lengthToRightBracket = i - leftBracket + 1;
				string.erase(leftBracket, lengthToRightBracket);
				i = leftBracket;
				break;
			}
		}
	}
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

	unsigned int amountOfWords = words.size();
	if (amountOfWords == 0)
	{
		wcout << "Add some words at first" << endl;
		_wsystem(L"pause");
		return L"";
	}
	else
	{
		int randomIndex = radnomNumber() % amountOfWords;
		return words[randomIndex];
	}
}

void testFunction(wstring language)
{
	wstring languageDirrectory = globalPath + L"\\" + wstringStandartForm(language) + L"Words";
	createDirrectory(languageDirrectory);

	int amountOfRepeats;
	wcout << L"Enter amount of test repetitions (greater than zero): ";
	wcin >> amountOfRepeats;
	wcin.ignore(LLONG_MAX, '\n');

	if (amountOfRepeats <= 0)
	{
		wcout << L"Amount of test repetitions can't be less than one." << endl;
		_wsystem(L"pause");
		return;
	}

	int amountOfCorrectAnswers = 0;
	for (int i = 0; i < amountOfRepeats; i++)
	{
		_wsystem(L"cls");

		wstring word = randomWordFrom(languageDirrectory);
		if (word == L"") return;

		wifstream wordFile(languageDirrectory + L"\\" + word);
		wordFile.imbue(std::locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wcout << word << L" is: ";

		wstring answer;
		getline(wcin, answer);
		wstringToLower(answer);

		double maxMatchRate = 0.0;
		vector<wstring> translations;
		for (wstring anotherLanguageWord; getline(wordFile, anotherLanguageWord);)
		{
			removeTextInBracket(anotherLanguageWord);
			translations.push_back(anotherLanguageWord);
			maxMatchRate = max(maxMatchRate, matchWords(anotherLanguageWord, answer));
		}
		wordFile.close();

		if (maxMatchRate == 1.0)
		{
			wcout << L"Absolutely correct!" << endl;

			if (translations.size() == 1)
			{
				wcout << L"Right is " << translations[0] << endl;
			}
			else
			{
				wcout << L"Right is one of:" << endl;
				for (int j = 0; j < translations.size(); j++)
					wcout << translations[j] << endl;
			}

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else if (maxMatchRate > 0.6)
		{
			wcout << L"Almost correct." << endl;

			if (translations.size() == 1)
			{
				wcout << L"Correct would be " << translations[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < translations.size(); j++)
					wcout << translations[j] << endl;
			}

			wcout << L"But it will be scored!" << endl;

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else
		{
			wcout << L"Nope." << endl;

			if (translations.size() == 1)
			{
				wcout << L"Correct would be " << translations[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < translations.size(); j++)
					wcout << translations[j] << endl;
			}

			wcout << L"It won't be scored!" << endl;

			_wsystem(L"pause");
		}
	}

	_wsystem(L"cls");
	wcout << L"You was right in " << amountOfCorrectAnswers * 100 / amountOfRepeats << L"% of cases" << endl;
	_wsystem(L"pause");
}

void addWordFunction(wstring language)
{
	wstring languageDirrectory = globalPath + L"\\" + wstringStandartForm(language) + L"Words";
	createDirrectory(languageDirrectory);

	wstring word;
	wcout << L"Enter the word: ";
	getline(wcin, word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}

	wstringToLower(word);

	wofstream wordFile(languageDirrectory + L"\\" + word);
	wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	wstring translation = L" ";
	vector<wstring> translations;
	while (translation.length() > 0)
	{
		wcout << L"Enter a translation (if you want to exit just leave it empty): ";
		getline(wcin, translation);
		wstringToLower(translation);

		if (translation.length() > 0)
		{
			translations.push_back(translation);
			wordFile << translation << endl;
		}
	}
	wordFile.close();

	if (translations.size() == 0)
	{
		wcout << "No one translations haven't entered." << endl;
		_wsystem(L"pause");

		wstring command = L"del " + languageDirrectory + L"\\" + word;
		_wsystem(&command[0]);
		return;
	}
}

void addTranslationFunction(wstring language)
{
	wstring wordDirrectory = globalPath + L"\\" + wstringStandartForm(language) + L"Words";
	createDirrectory(wordDirrectory);

	wstring word;
	wcout << L"Enter the word you want to add the translation for: ";
	getline(wcin, word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}

	wstringToLower(word);
	wordDirrectory += L"\\" + word;

	struct _stat buf;
	if (_wstat(&wordDirrectory[0], &buf) != 0)
	{
		wcout << L"This word doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wofstream wordFile(wordDirrectory, ios::app);
	wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	wstring translation = L" ";
	while (translation.length() > 0)
	{
		wcout << L"Enter a translation (if you want to exit just leave it empty): ";
		getline(wcin, translation);
		wstringToLower(translation);

		if (translation.length() > 0)
			wordFile << translation << endl;
	}
	wordFile.close();
}

void removeTranslationFunction(wstring language)
{
	wstring wordDirrectory = globalPath + L"\\" + wstringStandartForm(language) + L"Words";
	createDirrectory(wordDirrectory);

	wstring word;
	wcout << L"Enter the word you want to remove the translation for: ";
	getline(wcin, word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}

	wstringToLower(word);
	wordDirrectory += L"\\" + word;

	struct _stat buf;
	if (_wstat(&wordDirrectory[0], &buf) != 0)
	{
		wcout << L"This word doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	vector<wstring> translations;
	wifstream wordFileR(wordDirrectory);
	wordFileR.imbue(locale(wordFileR.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for(wstring translation; getline(wordFileR, translation);)
		translations.push_back(translation);
	wordFileR.close();
	

	wstring translationToRemove = L" ";
	while (translationToRemove.length() > 0)
	{
		wcout << L"Enter the translation you want to remove (if you want to exit just leave it empty): ";
		getline(wcin, translationToRemove);
		wstringToLower(translationToRemove);

		vector<wstring>::iterator positionOfTranslationToRemove = find(translations.begin(), translations.end(), translationToRemove);
		if (positionOfTranslationToRemove != translations.end())
		{
			translations.erase(positionOfTranslationToRemove);

			if (translations.size() == 0)
			{
				wcout << L"You have removed all existed translations. The word will be deleted." << endl;
				_wsystem(L"pause");

				wstring command = L"del " + wordDirrectory;
				_wsystem(&command[0]);
				return;
			}
		}
		else
		{
			wcout << L"The translation doesn't exist. Try again." << endl;
		}
	}

	wofstream wordFileW(wordDirrectory);
	wordFileW.imbue(locale(wordFileW.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for (int i = 0; i < translations.size(); i++)
		wordFileW << translations[i] << endl;
	wordFileW.close();
}

void rewriteTranslationFunction(wstring language)
{
	wstring wordDirrectory = globalPath + L"\\" + wstringStandartForm(language) + L"Words";
	createDirrectory(wordDirrectory);

	wstring word;
	wcout << L"Enter the word you want to rewrite translations for: ";
	getline(wcin, word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}

	wstringToLower(word);
	wordDirrectory += L"\\" + word;

	struct _stat buf;
	if (_wstat(&wordDirrectory[0], &buf) != 0)
	{
		wcout << L"This word doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wofstream wordFile(wordDirrectory);
	wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	wstring translation = L" ";
	vector<wstring> translations;
	while (translation.length() > 0)
	{
		wcout << L"Enter a translation (if you want to exit just leave it empty): ";
		getline(wcin, translation);
		wstringToLower(translation);

		if (translation.length() > 0)
		{
			translations.push_back(translation);
			wordFile << translation << endl;
		}
	}
	wordFile.close();

	if (translations.size() == 0)
	{
		wcout << "No one translations haven't entered. The word will be removed." << endl;
		_wsystem(L"pause");

		wstring command = L"del " + wordDirrectory;
		_wsystem(&command[0]);
		return;
	}
}

void deleteWordFunction(wstring language)
{
	wstring wordDirrectory = globalPath + L"\\" + wstringStandartForm(language) + L"Words";
	createDirrectory(wordDirrectory);

	wstring word;
	wcout << L"Enter the word you want to delete: ";
	getline(wcin, word);
	wstringToLower(word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}

	wstringToLower(word);
	wordDirrectory += L"\\" + word;

	struct _stat buf;
	if (_wstat(&wordDirrectory[0], &buf) != 0)
	{
		wcout << L"This word doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wstring command = L"del " + wordDirrectory;
	_wsystem(&command[0]);

	wcout << "Done!" << endl;
	_wsystem(L"pause");
	return;
}

void firstLanguageToSecondLanguageTest() { testFunction(firstLanguage); }
void secondLanguageToFirstLanguageTest() { testFunction(secondLanguage); }
void testingOption()
{
	vector<wstring> testingTypes = { wstringStandartForm(firstLanguage) + L" to " + secondLanguage , wstringStandartForm(secondLanguage) + L" to " + firstLanguage };
	vector<void (*)()> testingFunctions = { firstLanguageToSecondLanguageTest, secondLanguageToFirstLanguageTest };
	wconsoleMenu testingTypeMenu(testingTypes, testingFunctions);
	testingTypeMenu.select();
}

void addFirstLanguageWord() { addWordFunction(firstLanguage); }
void addSecondLanguagehWord() { addWordFunction(secondLanguage); }
void addWordOption()
{
	vector<wstring> languages = { wstringStandartForm(firstLanguage), wstringStandartForm(secondLanguage) };
	vector<void (*)()> addWordFunctions = { addFirstLanguageWord, addSecondLanguagehWord };
	wconsoleMenu languageOfWord(languages, addWordFunctions);
	languageOfWord.select();
}


void addTranslationForFirstLanguageFunction() { addTranslationFunction(firstLanguage); }
void removeTranslationForFirstLanguageFunction() { removeTranslationFunction(firstLanguage); }
void rewriteTranslationForFirstLanguageFunction() { rewriteTranslationFunction(firstLanguage); }
void updateFirstLanguageWord() 
{
	vector<wstring> options = { L"Add translation", L"Remove translation", L"Rewrite translations" };
	vector<void (*)()> updateWordFunctions = { addTranslationForFirstLanguageFunction, removeTranslationForFirstLanguageFunction, rewriteTranslationForFirstLanguageFunction };
	wconsoleMenu languageOfWord(options, updateWordFunctions);
	languageOfWord.select();
}

void addTranslationForSecondLanguageFunction() { addTranslationFunction(secondLanguage); }
void removeTranslationForSecondLanguageFunction() { removeTranslationFunction(secondLanguage); }
void rewriteTranslationForSecondLanguageFunction() { rewriteTranslationFunction(secondLanguage); }
void updateSecondLanguagehWord() 
{
	vector<wstring> options = { L"Add translation", L"Remove translation", L"Rewrite translations" };
	vector<void (*)()> updateWordFunctions = { addTranslationForSecondLanguageFunction, removeTranslationForSecondLanguageFunction, rewriteTranslationForSecondLanguageFunction };
	wconsoleMenu languageOfWord(options, updateWordFunctions);
	languageOfWord.select();
}

void updateWordOption()
{
	vector<wstring> languages = { wstringStandartForm(firstLanguage), wstringStandartForm(secondLanguage) };
	vector<void (*)()> updateWordFunctions = { updateFirstLanguageWord, updateSecondLanguagehWord };
	wconsoleMenu languageOfWord(languages, updateWordFunctions);
	languageOfWord.select();
}

void deleteFirstLanguageWord() { deleteWordFunction(firstLanguage); }
void deleteSecondLanguagehWord() { deleteWordFunction(secondLanguage); }
void deleteWordOption()
{
	vector<wstring> languages = { wstringStandartForm(firstLanguage), wstringStandartForm(secondLanguage) };
	vector<void (*)()> deleteWordFunctions = { deleteFirstLanguageWord, deleteSecondLanguagehWord };
	wconsoleMenu languageOfWord(languages, deleteWordFunctions);
	languageOfWord.select();
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

	vector<wstring> options = { L"Testing" , L"Add word" , L"Update word", L"Delete word" };
	vector<void (*)()> functions = { testingOption, addWordOption, updateWordOption, deleteWordOption };
	wconsoleMenu languageHelperMenu(options, functions, true);
	languageHelperMenu.select();
}