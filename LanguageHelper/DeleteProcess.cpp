#include "DeleteProcess.h"

void deleteWordFunction(wstring word)
{
	wstring wordDirrectory = languageDirrectory + L"\\" + word;

	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	_wremove(wordDirrectory.c_str());
	wcout << "Done!" << endl;
	_wsystem(L"pause");
	return;
}

void deleteWordOption(wstring optionName)
{
	updateOption = optionName;

	vector<wstring> languages = { firstLanguage, secondLanguage };
	vector<void (*)(wstring)> functions = functionMultiplier(displayAllSavedWords, languages.size());
	wconsoleMenu deleteM(languages, functions, L"Select the language of the word you want to delete", L"I changed my mind. Back, please");
	ignore = deleteM.singleSelect();
}