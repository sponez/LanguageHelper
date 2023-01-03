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

mt19937 radnomNumber((unsigned int)time(0));

bool isPathExist(wstring path);
void wordToLowerCase(wstring& word);
void capitalizedWord(wstring& word);
void removeTextInBracket(wstring& string);
void getWords(wstring path, vector<wstring>& emptyList);
void vectorDifference(vector<wstring>& first, vector<wstring>& second, vector<wstring>& result);
wstring randomWstring(vector<wstring>& wstringArray);
vector<void (*)(wstring&)> functionMultiplier(void (*function)(wstring&), short amount);
void shuffleVector(vector<wstring>& vector);
void getVectorFromWfile(wstring filePath, vector<wstring>& emptyVector, bool removeBracket = false);
void saveVectorToWfile(wstring filePath, vector<wstring>& sourseVector);
void removeWordFromWfile(wstring filePath, wstring line);
void getMapFromWfile(wstring filePath, map<wstring, int>& emptyMap);
void saveMapToWfile(wstring filePath, map<wstring, int>& sourseMap);
void printVector(vector<wstring> vector, wstring separator = L"\n", bool newLineAfter = true);
void printTranslations(vector<wstring> translations);
bool sucsessFeedback(wstring answer, vector<wstring> translations);
void addAllPairsCorrespondencesToSetFrom(wstring path, set<pair<wstring, wstring>>& correspondences, bool reverseOrder = false);
bool contains(vector<wstring>& vector, wstring& element);
bool askQuestion(wstring question);