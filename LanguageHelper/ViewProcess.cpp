#include "ViewProcess.h"

wstring currentLanguage;
wstring currentStage;

void wordViewer(wstring& word)
{
	vector <wstring> translations;

	wcout << L"Word: " << word << L'\n';

	getVectorFromWfile(
		ProgramDirectories::getPathToFile(
			word,
			currentLanguage,
			currentStage
		),
		translations
	);

	if (translations.size() > 1) {
		wcout << L"Translations:" << L'\n';
		printVector(translations, L"\n", 1);
	}
	else {
		wcout << L"Translation: ";
		printVector(translations);
	}

	_wsystem(L"pause");
}

void displaySavedWords(wstring& stage)
{
	vector<wstring> words;
	vector<void (*)(wstring&)> functions;
	wconsoleMenu displayWords;
	wstring selectText;

	currentStage = stage;

	getEntries(ProgramDirectories::getPathToDirectory(currentLanguage, currentStage), words);
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

	currentStage.clear();
}

void wordsStageToView(wstring& language)
{
	vector<wstring> stageOfWords = { ProgramDirectories::stages.unlearned , ProgramDirectories::stages.learned };
	vector<void (*)(wstring&)> functions = functionMultiplier(displaySavedWords, stageOfWords.size());
	wstring selectText = L"Select a stage";
	wstring exitText = L"Back";
	wconsoleMenu testingTypeMenu(stageOfWords, functions, selectText, exitText);

	currentLanguage = language;
	testingTypeMenu.cyclicSelect();
	currentLanguage.clear();
}

void wordsLanguageToView(wstring&)
{
	vector<wstring> languages = { ProgramDirectories::languages.native , ProgramDirectories::languages.target };
	vector<void (*)(wstring&)> functions = functionMultiplier(wordsStageToView, languages.size());
	wstring selectText = L"Select a language";
	wstring exitText = L"Back";
	wconsoleMenu testingTypeMenu(languages, functions, selectText, exitText);

	testingTypeMenu.cyclicSelect();
}