#include "ProgramDirectories.h"

wstring ProgramDirectories::getUsername()
{
	wchar_t username[255];
	DWORD username_len = 255;
	GetUserNameW(username, &username_len);

	return wstring(username);
}

void ProgramDirectories::getGlobalPath()
{
	globalPath = L"C:\\Users\\" + getUsername() + L"\\AppData\\Local\\Language Helper";
}

void ProgramDirectories::getLanguagesConfigFile()
{
	programFiles.languagesConfig = L"languages.cfg";
}

void ProgramDirectories::getLanguages()
{
	wifstream savedLanguagesFile(getPathToFile(programFiles.languagesConfig));
	savedLanguagesFile.imbue(std::locale(savedLanguagesFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	if (savedLanguagesFile.is_open())
	{
		savedLanguagesFile >> languages.native >> languages.target;
		savedLanguagesFile.close();
	}
	else
	{
		languageInitialization();
	}
}

void ProgramDirectories::getStages()
{
	stages.unlearned = L"Unlearned";
	stages.learned = L"Learned";
}

void ProgramDirectories::getProgressFiles()
{
	programFiles.successSave = L"CorrectAnswers.save";
	programFiles.usedWords = L"UsedWords.save";
	programFiles.unpassedWords = L"UnpassedWords.save";
}

void ProgramDirectories::getPropertiesFile()
{
	programFiles.properties = L"properties.cfg";
}

void ProgramDirectories::saveDefaulProperties()
{
	programProperties.correctAnswersToDelete.value = INT_MAX;
	programProperties.millisecondsToAnswerForCharacter.value = INT_MAX;
	programProperties.endlessTesting.value = 0;
	programProperties.randomTesting.value = 0;
	programProperties.windowWidth.value = 640;
	programProperties.windowHeight.value = 480;
	programProperties.fontWeight.value = 18;
	programProperties.fontWidth.value = 18;
	programProperties.fontHeight.value = 18;
	saveProperties();
}

void ProgramDirectories::getProperties()
{
	programProperties.correctAnswersToDelete.name = L"correctAnswersToDelete";
	programProperties.millisecondsToAnswerForCharacter.name = L"millisecondsToAnswerForCharacter";
	programProperties.endlessTesting.name = L"endlessTesting";
	programProperties.randomTesting.name = L"randomTesting";
	programProperties.windowWidth.name = L"windowWidth";
	programProperties.windowHeight.name = L"windowHeight";
	programProperties.fontWeight.name = L"fontWeight";
	programProperties.fontWidth.name = L"fontWidth";
	programProperties.fontHeight.name = L"fontHeight";

	wifstream propertiesFile(getPathToFile(programFiles.properties));
	propertiesFile.imbue(std::locale(propertiesFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	if (propertiesFile.is_open())
	{
		vector<Property*> properties = programProperties.getPropertiesList();

		for (int i = 0; i < properties.size(); i++)
		{
			wstring propertyNameFromFile;
			int propertyValueFromFile;

			if (propertiesFile >> propertyNameFromFile && propertyNameFromFile == properties[i]->name && propertiesFile >> propertyValueFromFile)
			{
				properties[i]->value = propertyValueFromFile;
			}
			else
			{
				propertiesFile.close();
				saveDefaulProperties();
				return;
			}
		}

		propertiesFile.close();
	}
	else
	{
		saveDefaulProperties();
		return;
	}
}

void ProgramDirectories::saveProperties()
{
	vector<Property*> properties = programProperties.getPropertiesList();

	wofstream propertiesFile(getPathToFile(programFiles.properties));
	propertiesFile.imbue(std::locale(propertiesFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	for (int i = 0; i < properties.size(); i++)
	{
		propertiesFile << properties[i]->name << L' ' << properties[i]->value << endl;
	}

	propertiesFile.close();
}

void ProgramDirectories::languageInitialization()
{
	wcout << L"Global initializing." << endl;
	wcout << L"Please, enter the native language: ";
	wcin >> languages.native;
	capitalizedWord(languages.native);

	wcout << L"Please, enter the target language: ";
	wcin >> languages.target;
	wcin.ignore(LLONG_MAX, L'\n');
	capitalizedWord(languages.target);

	wofstream configFile(getPathToFile(programFiles.languagesConfig));
	configFile.imbue(std::locale(configFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	configFile << languages.native << endl << languages.target;
	configFile.close();
}

void ProgramDirectories::createDirrectory(wstring path)
{
	struct _stat buf;
	if (_wstat(&path[0], &buf) != 0)
	{
		wstring command = L"md \"" + path + L'\"';
		_wsystem(command.c_str());
	}
}

void ProgramDirectories::checkProgressFiles()
{
	for (wstring checkLanguage : { languages.native , languages.target })
	{
		vector<wstring> words;
		map<wstring, int> successSaveMap;
		vector<wstring> unpassedWordsArray;
		vector<wstring> usedWordsArray;

		getEntries(getPathToDirectory(checkLanguage, stages.unlearned), words);

		getMapFromWfile(getPathToFile(programFiles.successSave, checkLanguage), successSaveMap);
		for (map<wstring, int>::iterator mapIt = successSaveMap.begin(); mapIt != successSaveMap.end(); mapIt++)
		{
			if (find(words.begin(), words.end(), mapIt->first) == words.end())
			{
				wcout << L"Progress files have broken. Progress lost." << endl;
				_wsystem(L"pause");

				_wremove(getPathToFile(programFiles.successSave, checkLanguage).c_str());
				_wremove(getPathToFile(programFiles.unpassedWords, checkLanguage).c_str());
				_wremove(getPathToFile(programFiles.usedWords, checkLanguage).c_str());
			}
		}

		getVectorFromWfile(getPathToFile(programFiles.unpassedWords, checkLanguage), unpassedWordsArray);
		for (vector<wstring>::iterator vecIt = unpassedWordsArray.begin(); vecIt != unpassedWordsArray.end(); vecIt++)
		{
			if (find(words.begin(), words.end(), *vecIt) == words.end())
			{
				wcout << L"Progress files have broken. Progress lost." << endl;
				_wsystem(L"pause");

				_wremove(getPathToFile(programFiles.successSave, checkLanguage).c_str());
				_wremove(getPathToFile(programFiles.unpassedWords, checkLanguage).c_str());
				_wremove(getPathToFile(programFiles.usedWords, checkLanguage).c_str());
			}
		}

		getVectorFromWfile(getPathToFile(programFiles.usedWords, checkLanguage), usedWordsArray);
		for (vector<wstring>::iterator vecIt = usedWordsArray.begin(); vecIt != usedWordsArray.end(); vecIt++)
		{
			if (find(words.begin(), words.end(), *vecIt) == words.end())
			{
				wcout << L"Progress files have broken. Progress lost." << endl;
				_wsystem(L"pause");

				_wremove(getPathToFile(programFiles.successSave, checkLanguage).c_str());
				_wremove(getPathToFile(programFiles.unpassedWords, checkLanguage).c_str());
				_wremove(getPathToFile(programFiles.usedWords, checkLanguage).c_str());
			}
		}
	}
}

wstring ProgramDirectories::getPathToDirectory(wstring language, wstring stage)
{
	wstring path;

	if (globalPath.empty())
	{
		wcout << L"Empty global path." << endl;
		_wsystem(L"pause");
		exit(INT_MIN);
	}

	path = globalPath;

	if (!language.empty() && language != languages.native && language != languages.target)
	{
		wcout << L"Invalid language." << endl;
		_wsystem(L"pause");
		exit(INT_MIN);
	}

	if (language.empty() && !stage.empty())
	{
		wcout << L"Stage without language." << endl;
		_wsystem(L"pause");
		exit(INT_MIN);
	}

	if (!stage.empty() && stage != stages.learned && stage != stages.unlearned)
	{
		wcout << L"Invalid stage." << endl;
		_wsystem(L"pause");
		exit(INT_MIN);
	}

	if (!language.empty()) { path += L"\\" + language; }
	if (!stage.empty()) { path += L"\\" + stage; }

	return path;
}

wstring ProgramDirectories::getPathToFile(wstring file, wstring language, wstring stage)
{
	if (file == programFiles.languagesConfig || file == programFiles.properties)
	{
		if (!language.empty())
		{
			wcout << L"Config with language." << endl;
			_wsystem(L"pause");
			exit(INT_MIN);
		}
		else
		{
			return getPathToDirectory() + L"\\" + file;
		}
	}
	else if (file == programFiles.successSave ||
		file == programFiles.unpassedWords ||
		file == programFiles.usedWords)
	{
		if (language.empty() || !stage.empty())
		{
			wcout << L"Invalid progress files directory." << endl;
			_wsystem(L"pause");
			exit(INT_MIN);
		}
		else
		{
			return getPathToDirectory(language) + L"\\" + file;
		}
	}
	else
	{
		if (language.empty() || stage.empty())
		{
			wcout << L"Invalid words directory" << endl;
			_wsystem(L"pause");
			exit(INT_MIN);
		}
		else
		{
			return getPathToDirectory(language, stage) + L"\\" + file;
		}
	}
}

void ProgramDirectories::removeWordFromProgramFiles(wstring word, wstring language)
{
	removeWordFromWfile(
		ProgramDirectories::getPathToFile(
			ProgramDirectories::programFiles.successSave,
			language
		),
		word
	);

	removeWordFromWfile(
		ProgramDirectories::getPathToFile(
			ProgramDirectories::programFiles.unpassedWords,
			language
		),
		word
	);

	removeWordFromWfile(
		ProgramDirectories::getPathToFile(
			ProgramDirectories::programFiles.usedWords,
			language
		),
		word
	);
}

wstring ProgramDirectories::reverseLanguage(wstring language)
{
	if (language != languages.native && language != languages.target)
	{
		wcout << L"Invalid language for reverse" << endl;
		_wsystem(L"pause");
		exit(INT_MIN);
	}

	return (language == languages.native) ? languages.target : languages.native;
}

wstring ProgramDirectories::reverseStage(wstring stage)
{
	if (stage != stages.unlearned && stage != stages.learned)
	{
		wcout << L"Invalid stage for reverse" << endl;
		_wsystem(L"pause");
		exit(INT_MIN);
	}

	return (stage == stages.unlearned) ? stages.learned : stages.unlearned;
}

void setProfileName(wstring& profileName) {
	ProgramDirectories::profileName = profileName;
}

void addProfileNewProfile(wstring&) {
	wcout << L"Enter a new profile name: ";
	wcin >> ProgramDirectories::profileName;
}

void ProgramDirectories::getProgramDirectories()
{
	getGlobalPath();

	vector<wstring> profiles;
	getEntries(ProgramDirectories::globalPath, profiles);
	profiles.push_back(L"Add new");


	vector<void (*)(wstring&)> profileSet = functionMultiplier(setProfileName, profiles.size() - 1);
	profileSet.push_back(addProfileNewProfile);
	wstring selectText = L"Select a profile";
	wconsoleMenu profileSelect(profiles, profileSet, selectText);

	profileSelect.singleSelect();

	globalPath += L'\\' + profileName;

	createDirrectory(getPathToDirectory());

	getLanguagesConfigFile();
	getLanguages();
	createDirrectory(getPathToDirectory(languages.native));
	createDirrectory(getPathToDirectory(languages.target));

	getStages();
	createDirrectory(getPathToDirectory(languages.native, stages.unlearned));
	createDirrectory(getPathToDirectory(languages.native, stages.learned));
	createDirrectory(getPathToDirectory(languages.target, stages.unlearned));
	createDirrectory(getPathToDirectory(languages.target, stages.learned));

	getProgressFiles();
	checkProgressFiles();

	getPropertiesFile();
	getProperties();
}
