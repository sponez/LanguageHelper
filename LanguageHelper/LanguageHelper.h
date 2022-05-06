#pragma once
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
mt19937 randomNumber(time(0));

void createDirectory(wstring path);

void wstringToLower(wstring& s);

wstring wstringStandartForm(wstring s);

void removeTextInBracket(wstring& string);

double findMaxRate(vector<vector<bool>>& matrix, int curri, int currj);

double matchWords(wstring mainWord, wstring secondaryWord);

void testFunction(wstring language);

void displayAllSecondLanguageSavedWords(wstring language);

void displayTranslatesForParticularWord(wstring language);

void addWordFunction(wstring language);

void addTranslationFunction(wstring language);

void removeTranslationFunction(wstring language);

void rewriteTranslationFunction(wstring language);

void deleteWordFunction(wstring language);

void firstLanguageToSecondLanguageTest();

void secondLanguageToFirstLanguageTest();

void testingOption();

void displayAllFirstLanguageSavedWords();

void displayTranslatesForParticularFirstLanguageWord();

void viewFirstLanguageWords();

void displayAllSecondLanguageSavedWords();

void displayTranslatesForParticularSecondLanguageWord();

void viewSecondLanguagehWords();

void viewWordsOption();

void addFirstLanguageWord();

void addSecondLanguagehWord();

void addWordOption();

void addTranslationForFirstLanguageFunction();

void removeTranslationForFirstLanguageFunction();

void rewriteTranslationForFirstLanguageFunction();

void updateFirstLanguageWord();

void addTranslationForSecondLanguageFunction();

void removeTranslationForSecondLanguageFunction();

void rewriteTranslationForSecondLanguageFunction();

void updateSecondLanguagehWord();

void updateWordOption();

void deleteFirstLanguageWord();

void deleteSecondLanguagehWord();

void deleteWordOption();

int main(int argc, wchar_t* argv[]);