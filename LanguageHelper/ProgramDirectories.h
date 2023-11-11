#pragma once
#pragma warning(disable : 4996)
#include "AdditionalFunctions.h"
#include <map>
using namespace std;

class ProgramDirectories
{
public:
	struct Property
	{
		wstring name;
		int value;
	};

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
		Property correctAnswersToDelete;
		Property millisecondsToAnswerForCharacter;
		Property endlessTesting;
		Property randomTesting;

		Property windowWidth;
		Property windowHeight;

		Property fontWeight;
		Property fontWidth;
		Property fontHeight;

		vector<Property*> getPropertiesList()
		{
			return {
				&correctAnswersToDelete ,
				&millisecondsToAnswerForCharacter ,
				&endlessTesting ,
				&randomTesting ,
				&windowWidth ,
				&windowHeight ,
				&fontWeight ,
				&fontWidth ,
				&fontHeight
			};
		}
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
	inline static wstring profileName;
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