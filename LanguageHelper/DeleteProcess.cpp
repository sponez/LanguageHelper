#include "DeleteProcess.h"

void deleteWordFunction(wstring language)
{
	wstring wordDirrectory = globalPath + L"\\" + language + L"Words";
	createDirrectory(wordDirrectory);

	wstring word;
	wcout << L"Enter the word you want to delete: ";
	getline(wcin, word);
	wstringToLower(word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}

	wstringToLower(word);
	wordDirrectory += L"\\" + word;

	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wstring command = L"del " + wordDirrectory;
	_wsystem(&command[0]);

	wcout << "Done!" << endl;
	_wsystem(L"pause");
	return;
}

void deleteWordOption(wstring)
{
	vector<wstring> languages = { firstLanguage, secondLanguage };
	vector<void (*)(wstring)> deleteWordFunctions = { deleteWordFunction, deleteWordFunction };
	wconsoleMenu deleteM(L"Select the language of the word you want to delete", languages, deleteWordFunctions, L"I changed my mind. Back, please");
	deleteM.select();
}