#pragma once
#pragma warning(disable : 4996)
#include <iostream>
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
wstring languageDirrectory;
wstring updateOption;

void createDirrectory(wstring&);
bool isFileExist(wstring& path);
void wstringToLower(wstring&);
void wstringStandartForm(wstring&);
void removeTextInBracket(wstring&);
double DamerauLevenshteinDistance(wstring&, wstring&);
void getWords(wstring&, vector<wstring>&);
void getWords(wstring&, vector<wstring>&, wstring);
void displayFilteredWords(wstring&);
void displayAllSavedWords(wstring);
wstring randomWordFrom(wstring&);