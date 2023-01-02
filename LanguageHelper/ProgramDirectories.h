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
#include <windows.h>
using namespace std;

class ProgramDirectories
{
private:
	struct Languages
	{
		wstring native;
		wstring target;
	};

	struct Stages
	{
		wstring unlearned;
		wstring learned;
	};

	struct ProgramFiles
	{
		wstring languagesConfig;
		wstring successSave;
		wstring usedWords;
		wstring unpassedWords;
	};

	static wstring getUsername();
	static void getGlobalPath();
	static void getLanguagesConfigFile();
	static void getLanguages();
	static void getStages();
	static void getProgressFiles();
	static void languageInitialization();
	static void createDirrectory(wstring path);

public:
	inline static wstring globalPath;
	inline static Languages languages;
	inline static Stages stages;
	inline static ProgramFiles programFiles;

	static wstring getPathToDirectory(wstring language = wstring(), wstring stage = wstring());
	static wstring getPathToFile(wstring file, wstring language = wstring(), wstring stage = wstring());
	static wstring reverseLanguage(wstring language);
	static wstring reverseStage(wstring stage);
	static void getProgramDirectories();
};