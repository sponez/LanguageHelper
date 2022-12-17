#pragma once
#pragma warning(disable : 4996)
#include <iostream>
#include <codecvt>
#include <fstream>
#include <filesystem>
#include <vector>
#include <set>
#include <algorithm>
#include <string>
#include <cmath>
#include <random>
#include "wconsoleMenu.h"
using namespace std;

wstring globalPath;
wstring firstLanguage;
wstring secondLanguage;
mt19937 radnomNumber((unsigned int)time(0));

void createDirrectory(wstring& path);
bool isFileExist(wstring& path);
void wstringToLower(wstring& s);
void wstringStandartForm(wstring& s);
void removeTextInBracket(wstring& string);
void getWords(wstring path, vector<wstring>& emptyList);
void getTranslations(wstring path, vector<wstring>& emptyList);
void vectorDifference(vector<wstring>& first, vector<wstring>& second, vector<wstring>& result);
wstring randomWordFrom(wstring& path, bool saveWord = true);
vector<void (*)(wstring&)> functionMultiplier(void (*function)(wstring&), short amount);
void shuffleVector(vector<wstring>& vector);