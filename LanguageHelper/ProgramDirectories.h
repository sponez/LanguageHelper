#pragma once
#pragma warning(disable : 4996)
#include "AdditionalFunctions.h"
#include <map>
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
		wstring properties;
	};

	struct ProgramProperties
	{
		short correctAnswersToDelete;
	};

	static wstring getUsername();
	static void getGlobalPath();
	static void getLanguagesConfigFile();
	static void getLanguages();
	static void getStages();
	static void getProgressFiles();
	static void getPropertiesFile();
	static void getProperties();
	static void saveDefaulProperties();
	static void languageInitialization();
	static void createDirrectory(wstring path);
	static void checkProgressFiles();

public:
	inline static wstring globalPath;
	inline static Languages languages;
	inline static Stages stages;
	inline static ProgramFiles programFiles;
	inline static ProgramProperties programProperties;

	static wstring getPathToDirectory(wstring language = wstring(), wstring stage = wstring());
	static wstring getPathToFile(wstring file, wstring language = wstring(), wstring stage = wstring());
	static void removeWordFromProgramFiles(wstring word, wstring language);
	static wstring reverseLanguage(wstring language);
	static wstring reverseStage(wstring stage);
	static void getProgramDirectories();
	static void saveProperties();
};