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

	wstring command = L"del " + wordDirrectory;
	_wsystem(&command[0]);

	wcout << "Done!" << endl;
	_wsystem(L"pause");
	return;
}

void deleteWordOption(wstring optionName)
{
	updateOption = optionName;

	vector<wstring> languages = { firstLanguage, secondLanguage };
	vector<void (*)(wstring)> deleteWordFunctions = { displayAllSavedWords, displayAllSavedWords };
	wconsoleMenu deleteM(L"Select the language of the word you want to delete", languages, deleteWordFunctions, L"I changed my mind. Back, please");
	deleteM.select();
}