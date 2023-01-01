#include "ManageWordsProcess.h"

wstring currentLanguage;
wstring currentStage;
wstring* currentWordPointer;
wstring* currentTranslationPointer;

void convertWordsFromAnotherLanguage(wstring&)
{
	set<pair<wstring, wstring>> correspondences;
	addAllPairsCorrespondencesToSetFrom(ProgramDirectories::getPathToDirectory(currentLanguage, ProgramDirectories::stages.unlearned), correspondences);
	addAllPairsCorrespondencesToSetFrom(ProgramDirectories::getPathToDirectory(ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned), correspondences, true);
	addAllPairsCorrespondencesToSetFrom(ProgramDirectories::getPathToDirectory(ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.learned), correspondences, true);

	if (!correspondences.empty())
	{
		wstring currentWord = correspondences.begin()->first;
		vector<wstring> translations;

		for (set<pair<wstring, wstring>>::iterator correspondenceIt = correspondences.begin(); correspondenceIt != correspondences.end(); correspondenceIt++)
		{
			if (correspondenceIt->first != currentWord)
			{
				saveVectorToWfile(ProgramDirectories::getPathToFile(currentWord, currentLanguage, ProgramDirectories::stages.unlearned), translations);
				currentWord = correspondenceIt->first;
				translations.clear();
			}

			translations.push_back(correspondenceIt->second);
		}
		saveVectorToWfile(ProgramDirectories::getPathToFile(currentWord, currentLanguage, ProgramDirectories::stages.unlearned), translations);
	}
}

void getRandomWordsFromLearned(wstring&)
{
	int amount;
	vector<wstring> allLearnedWords;

	getWords(ProgramDirectories::getPathToDirectory(currentLanguage, ProgramDirectories::stages.learned), allLearnedWords);
	wcout << L"Enter amount of words from 1 to " << allLearnedWords.size() << L": ";
	if (wcin >> amount)
	{
		wcin.ignore(LLONG_MAX, '\n');

		if (amount <= 0)
		{
			_wsystem(L"cls");
			wcout << L"Invalid operation." << endl;
			_wsystem(L"pause");
			return;
		}

		if (amount > allLearnedWords.size())
		{
			_wsystem(L"cls");
			wcout << L"Too much. Will moved all words." << endl;
			_wsystem(L"pause");

			amount = allLearnedWords.size();
		}

		for (int i = 0; i < amount; i++)
		{
			wstring wordToMove = randomWstring(allLearnedWords);
			moveWfile(ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.learned),
				ProgramDirectories::getPathToDirectory(currentLanguage, ProgramDirectories::stages.unlearned));

			ignore = remove(allLearnedWords.begin(), allLearnedWords.end(), wordToMove);
		}

		_wsystem(L"cls");
		wcout << L"Done!" << endl;
		_wsystem(L"pause");
	}
	else
	{
		wcin.ignore(LLONG_MAX, '\n');

		_wsystem(L"cls");
		wcout << L"Something went wrong." << endl;
		_wsystem(L"pause");
		return;
	}
}

void addWordFunction(wstring&)
{
	wstring word;
	wstring pathToWord;
	wcout << L"Enter the word: ";
	getline(wcin, word);
	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}
	wstringToLower(word);

	pathToWord = ProgramDirectories::getPathToFile(word, currentLanguage, currentStage);
	if (isPathExist(pathToWord))
	{
		wcout << L"This word already exist." << endl;
		_wsystem(L"pause");
		return;
	}

	vector<wstring> translations;
	while (true)
	{
		wstring translation;

		wcout << L"Leave empty to exit." << endl;
		wcout << L"Enter a translation: ";

		getline(wcin, translation);
		if (translation.empty()) { break; }
		wstringToLower(translation);

		translations.push_back(translation);
	}

	if (translations.size() != 0) { saveVectorToWfile(pathToWord, translations); }
}

void deleteWordFunction(wstring&)
{
	_wremove(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage).c_str());
	wcout << "Done!" << endl;
	_wsystem(L"pause");
}

void moveToUnlearned(wstring&)
{
	moveWfile(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage),
		ProgramDirectories::getPathToDirectory(currentLanguage, ProgramDirectories::stages.unlearned));
	_wsystem(L"cls");
	wcout << "Done!" << endl;
	_wsystem(L"pause");
}

void addTranslationFunction(wstring&)
{
	wofstream wordFile(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage), ios::app);
	wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	wcout << L"Leave empty to exit." << endl;
	while (true)
	{
		wstring translation;
		wcout << L"Enter a translation: ";

		getline(wcin, translation);
		if (translation.empty()) { break; }
		wstringToLower(translation);

		wordFile << translation << endl;
	}

	wordFile.close();
}

void translationEditorFunction(wstring& optionName)
{
	wstring pathToWord = ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage);
	vector<wstring> translations;

	getVectorFromWfile(pathToWord, translations);
	if (translations.size() == 0)
	{
		wcout << "Translations not found! The word will be deleted." << endl;
		_wsystem(L"pause");
		_wremove(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage).c_str());
		return;
	}

	vector<wstring>::iterator positionOfTranslation = find(translations.begin(), translations.end(), *currentTranslationPointer);
	if (positionOfTranslation != translations.end())
	{
		if (optionName == L"Edit")
		{
			wconsoleMenu::consoleWstringEditor(*currentTranslationPointer);
			*positionOfTranslation = *currentTranslationPointer;
		}

		if (optionName == L"Delete")
		{
			translations.erase(positionOfTranslation);

			if (translations.size() == 0)
			{
				wcout << L"You have removed all existed translations. The word will be deleted." << endl;
				_wsystem(L"pause");
				_wremove(pathToWord.c_str());
				return;
			}
		}
	}
	else
	{
		wcout << L"The translation already doesn't exist" << endl;
		return;
	}

	saveVectorToWfile(pathToWord, translations);
}

void translatorEditor(wstring& translation)
{
	currentTranslationPointer = &translation;

	vector<wstring> editorOptions = { L"Edit" , L"Delete" };
	vector<void (*)(wstring&)> functions = functionMultiplier(translationEditorFunction, editorOptions.size());
	wconsoleMenu languageOfWord(editorOptions, functions, L"What do you want to do?", L"I changed my mind. Back, please");
	languageOfWord.singleSelect();

	currentTranslationPointer = nullptr;
}

void displayTranslations(wstring&)
{
	wstring pathToWord = ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage);
	short optionNum = 0;
	wstring filterText = wstring();
	bool skipFilter = false;
	do
	{
		vector<wstring> translations;
		vector<void (*)(wstring&)> functions;
		wconsoleMenu displayTranslations;

		getVectorFromWfile(pathToWord, translations);
		if (translations.size() == 0)
		{
			wcout << "Translations not found! The word will be deleted." << endl;
			_wsystem(L"pause");
			_wremove(pathToWord.c_str());
			return;
		}

		functions = functionMultiplier(translatorEditor, translations.size());

		if (translations.size() > 15)
		{
			displayTranslations = wconsoleMenu(translations, functions, L"Filter translations by characters: ");
			displayTranslations.singleSelectWithFilter(optionNum, filterText, skipFilter);
			skipFilter = true;
		}
		else
		{
			displayTranslations = wconsoleMenu(translations, functions, L"Select translation:");
			displayTranslations.singleSelect(optionNum);
		}

	} while (optionNum != -1);
}

void ranameWordFunction(wstring&)
{
	vector<wstring> translations;

	getVectorFromWfile(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage), translations);
	if (translations.size() == 0)
	{
		wcout << "Translations not found! The word is broken and will be deleted." << endl;
		_wsystem(L"pause");
		_wremove(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage).c_str());
		return;
	}

	_wremove(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage).c_str());
	wconsoleMenu::consoleWstringEditor(*currentWordPointer);
	saveVectorToWfile(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage), translations);
}

void wordEditor(wstring& word)
{
	currentWordPointer = &word;

	vector<wstring> editorOptions;
	vector<void (*)(wstring&)> functions;
	wconsoleMenu editor;

	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		editorOptions = { L"Rename" , L"View translations" , L"Add translations" , L"Delete" };
		functions = { ranameWordFunction , displayTranslations , addTranslationFunction , deleteWordFunction };
		editor = wconsoleMenu(editorOptions, functions, L"What do you want to do?", L"I changed my mind. Back, please");

		short optionNum = 0;
		do
		{
			editor.singleSelect(optionNum);
		} while (optionNum != -1 && optionNum != 3);
	}
	else
	{
		editorOptions = { L"View translations" , L"Add translations" , L"Move to unlearned", L"Delete" };
		functions = { displayTranslations , addTranslationFunction , moveToUnlearned , deleteWordFunction };
		editor = wconsoleMenu(editorOptions, functions, L"What do you want to do?", L"I changed my mind. Back, please");

		short optionNum = 0;
		do
		{
			editor.singleSelect(optionNum);
		} while (optionNum != -1 && optionNum != 2 && optionNum != 3);
	}

	currentWordPointer = nullptr;
}

void displayAllSavedWordsToEdit(wstring&)
{
	short optionNum = 0;
	wstring filterText = L"";
	bool skipFilter = false;
	do
	{
		vector<wstring> words;
		vector<void (*)(wstring&)> functions;
		wconsoleMenu displayWords;
		wstring selectText;

		getWords(ProgramDirectories::getPathToDirectory(currentLanguage, currentStage), words);
		if (words.size() == 0)
		{
			wcout << "Words not found!" << endl;
			_wsystem(L"pause");
			return;
		}

		functions = functionMultiplier(wordEditor, words.size());

		if (words.size() > 15)
		{
			selectText = (wstring)L"Filter " + to_wstring(words.size()) + (wstring)L" words by characters: ";
			displayWords = wconsoleMenu(words, functions, selectText);
			displayWords.singleSelectWithFilter(optionNum, filterText, skipFilter);
			skipFilter = true;
		}
		else
		{
			selectText = L"Select word:";
			displayWords = wconsoleMenu(words, functions, selectText);
			displayWords.singleSelect(optionNum);
		}

	} while (optionNum != -1);
}

void addNewOrEdit(wstring& stage)
{
	currentStage = stage;

	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		vector<wstring> options = { L"Edit saved words" , L"Add new word" , L"Get several random words from learned" ,
			(L"Convert words from " + ProgramDirectories::reverseLanguage(currentLanguage)) };
		vector<void (*)(wstring&)> functions = { displayAllSavedWordsToEdit , addWordFunction , getRandomWordsFromLearned , convertWordsFromAnotherLanguage };
		wconsoleMenu languageOfWord(options, functions, L"What do you want to do?", L"I changed my mind. Back, please");
		languageOfWord.cyclicSelect();
	}
	else
	{
		displayAllSavedWordsToEdit(currentStage);
	}

	currentStage.clear();
}

void wordsStageToManage(wstring& language)
{
	currentLanguage = language;

	vector<wstring> stageOfWords = { ProgramDirectories::stages.unlearned , ProgramDirectories::stages.learned };
	vector<void (*)(wstring&)> functions = functionMultiplier(addNewOrEdit, stageOfWords.size());
	wconsoleMenu editWordsMenu(stageOfWords, functions, L"Select the language of words you want to see", L"I changed my mind. Back, please");
	editWordsMenu.singleSelect();

	currentLanguage.clear();
}

void wordsLanguageToManage(wstring&)
{
	vector<wstring> languages = { ProgramDirectories::languages.native , ProgramDirectories::languages.target };
	vector<void (*)(wstring&)> functions = functionMultiplier(wordsStageToManage, languages.size());
	wconsoleMenu languageOfWord(languages, functions, L"Select the language of the saved words you want to manage", L"I changed my mind. Back, please");
	languageOfWord.singleSelect();
}