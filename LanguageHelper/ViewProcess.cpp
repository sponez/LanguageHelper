#include "AdditionalFunctions.h"
#include "wconsoleMenu.h"
#include "ViewProcess.h"

wstring currentLanguageDirrectory;

void wordViewer(wstring& word)
{
	wstring currentWord = word;
	wstring wordDirrectory = currentLanguageDirrectory + L"\\" + currentWord;
	vector <wstring> translations;

	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wcout << L"Word: " << currentWord << endl;
	wcout << L"Translations: ";

	wifstream wordFileR(wordDirrectory);
	wordFileR.imbue(locale(wordFileR.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for (wstring translation; getline(wordFileR, translation);)
		translations.push_back(translation);
	wordFileR.close();

	for (int i = 0; i < translations.size() - 1; i++)
		wcout << translations[i] << L", ";
	wcout << translations.back() << endl;

	_wsystem(L"pause");
}

void displayAllSavedWordsToView(wstring& language)
{
	currentLanguageDirrectory = globalPath + L"\\Unlearned\\" + language + L" Words";
	createDirrectory(currentLanguageDirrectory);

	vector<void (*)(wstring&)> functions;
	wconsoleMenu displayWords;
	wstring selectText;

	vector<wstring> words;
	getWords(currentLanguageDirrectory, words);
	if (words.size() == 0)
	{
		wcout << "Words not found!" << endl;
		_wsystem(L"pause");
		return;
	}
	functions = functionMultiplier(wordViewer, words.size());

	if (words.size() > 15)
	{
		short optionNum = 0;
		wstring filterText = L"";
		bool skipFilter = false;

		selectText = (wstring)L"Filter " + to_wstring(words.size()) + (wstring)L" words by characters: ";
		displayWords = wconsoleMenu(words, functions, selectText);

		do
		{
			displayWords.singleSelectWithFilter(optionNum, filterText, skipFilter);
			skipFilter = true;
		} while (optionNum != -1);
	}
	else
	{
		selectText = L"Select word:";
		displayWords = wconsoleMenu(words, functions, selectText);
		displayWords.cyclicSelect();
	}
}

void viewOption(wstring&)
{
	vector<wstring> languageOfWords = { firstLanguage, secondLanguage };
	vector<void (*)(wstring&)> functions = functionMultiplier(displayAllSavedWordsToView, languageOfWords.size());
	wconsoleMenu testingTypeMenu(languageOfWords, functions, L"Select the language of words you want to see", L"I changed my mind. Back, please");
	testingTypeMenu.singleSelect();
}