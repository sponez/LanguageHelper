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
#include "wconsoleMenu.h"
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
void displayAllSavedWords(wstring);
wstring randomWordFrom(wstring&);
vector<void (*)(wstring)> functionMultiplier(void (*function)(wstring), short);