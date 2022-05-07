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
using namespace std;

wstring globalPath;
wstring firstLanguage;
wstring secondLanguage;

void createDirrectory(wstring&);
bool isFileExist(wstring& path);
void wstringToLower(wstring&);
void wstringStandartForm(wstring&);
void removeTextInBracket(wstring&);
double findMaxRate(vector<vector<bool>>&, int, int);
double matchWords(wstring&, wstring&);
void getWords(wstring&, vector<wstring>&);
wstring randomWordFrom(wstring&);