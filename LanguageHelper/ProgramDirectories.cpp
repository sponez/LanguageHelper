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

void ProgramDirectories::languageInitialization()
{
	wcout << L"Global initializing." << endl;
	wcout << L"Please, enter the native language: ";
	wcin >> languages.native;

	wcout << L"Please, enter the target language: ";
	wcin >> languages.target;
	wcin.ignore(LLONG_MAX, L'\n');

	for (int i = 0; i < languages.native.length(); i++)
	{
		languages.native[i] = tolower(languages.native[i], locale(""));
	}
	languages.native[0] = toupper(languages.native[0], locale(""));
	
	for (int i = 0; i < languages.target.length(); i++)
	{
		languages.target[i] = tolower(languages.target[i], locale(""));
	}
	languages.target[0] = toupper(languages.target[0], locale(""));

	wofstream configFile(getPathToFile(programFiles.languagesConfig));
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
	if (file == programFiles.languagesConfig)
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

void ProgramDirectories::getProgramDirectories()
{
	getGlobalPath();
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
}