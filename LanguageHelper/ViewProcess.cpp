#include "ViewProcess.h"

void displayTranslationsFor(wstring word)
{
	wstring wordDirrectory = languageDirrectory + L"\\" + word;
	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wifstream wordFileR(wordDirrectory);
	wordFileR.imbue(locale(wordFileR.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for (wstring translation; getline(wordFileR, translation);)
		wcout << translation << endl;

	_wsystem(L"pause");
}

void viewWordsOption(wstring optionName)
{
	updateOption = optionName;

	vector<wstring> languages = { firstLanguage, secondLanguage };
	vector<void (*)(wstring)> functions = functionMultiplier(displayAllSavedWords, languages.size());
	wconsoleMenu languageOfWord(languages, functions, L"Select the language of the saved words you want to see", L"I changed my mind. Back, please");
	ignore = languageOfWord.singleSelect();
}