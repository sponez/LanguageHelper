#pragma warning(disable : 4996)
#include "AddProcess.h"

void addWordFunction(wstring language)
{
	wstring languageDirrectory = globalPath + L"\\" + language + L"Words";
	createDirrectory(languageDirrectory);

	wstring word;
	wcout << L"Enter the word: ";
	getline(wcin, word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}
	wstringToLower(word);

	wstring wordDirrectory = languageDirrectory + L"\\" + word;
	if (isFileExist(wordDirrectory))
	{
		wcout << L"This word already exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wofstream wordFile(languageDirrectory + L"\\" + word);
	wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	wstring translation = L" ";
	vector<wstring> translations;
	while (translation.length() > 0)
	{
		wcout << L"Enter a translation (if you want to exit just leave it empty): ";
		getline(wcin, translation);
		wstringToLower(translation);

		if (translation.length() > 0)
		{
			translations.push_back(translation);
			wordFile << translation << endl;
		}
	}
	wordFile.close();

	if (translations.size() == 0)
	{
		wcout << "No one translations haven't entered." << endl;
		_wsystem(L"pause");

		wstring wordDirrectory = languageDirrectory + L"\\" + word;
		_wremove(wordDirrectory.c_str());
		return;
	}
}

void addWordOption(wstring)
{
	vector<wstring> languages = { firstLanguage, secondLanguage };
	vector<void (*)(wstring)> functions = functionMultiplier(addWordFunction, languages.size());
	wconsoleMenu add(languages, functions, L"Select the language of the word you want to add", L"I changed my mind. Back, please");
	ignore = add.singleSelect();
}