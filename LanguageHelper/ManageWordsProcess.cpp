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

void addTranslationsManual(wstring& word, wstring& wordPath, vector<wstring>& translations)
{
	wcout << L"Leave empty to exit." << endl;
	while (true)
	{
		wstring translation;
		wcout << L"Enter a translation: ";

		getline(wcin, translation);
		if (translation.empty()) { break; }
		wstringToLower(translation);

		if (!contains(translations, translation)) { translations.push_back(translation); }
		else
		{
			wcout << L"Already exist." << endl;
			_wsystem(L"pause");
		}
	}

	if (translations.size() != 0)
	{
		saveVectorToWfile(wordPath, translations);

		for (wstring translation : translations)
		{
			removeTextInBracket(translation);

			vector<wstring> reverseTranslations;
			wstring reverseWordPath = ProgramDirectories::getPathToFile(translation, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);

			getVectorFromWfile(reverseWordPath, reverseTranslations);
			if (!contains(reverseTranslations, word))
			{
				reverseTranslations.push_back(word);
				saveVectorToWfile(reverseWordPath, reverseTranslations);
			}
		}
	}
}

void addWordFunction(wstring&)
{
	wstring word;
	wstring wordPath;
	wcout << L"Enter the word: ";
	getline(wcin, word);
	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}
	wstringToLower(word);

	wordPath = ProgramDirectories::getPathToFile(word, currentLanguage, currentStage);
	if (isPathExist(wordPath))
	{
		wcout << L"This word already exist." << endl;
		_wsystem(L"pause");
		return;
	}

	vector<wstring> translations;
	addTranslationsManual(word, wordPath, translations);
}

void deleteTranslationManual(wstring& word, wstring& wordPath, vector<wstring>& translations, wstring& translationToDelete)
{
	vector<wstring> reverseTranslations;
	wstring reverseWordPath = ProgramDirectories::getPathToFile(translationToDelete, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);

	getVectorFromWfile(reverseWordPath, reverseTranslations);
	if (contains(reverseTranslations, word))
	{
		vector<wstring>::iterator it = find(reverseTranslations.begin(), reverseTranslations.end(), word);
		reverseTranslations.erase(it);

		if (!reverseTranslations.empty()) { saveVectorToWfile(reverseWordPath, reverseTranslations); }
		else { _wremove(reverseWordPath.c_str()); }
	}

	if (contains(translations, translationToDelete))
	{
		vector<wstring>::iterator it = find(translations.begin(), translations.end(), translationToDelete);
		translations.erase(it);
	}
}

void deleteWordFunction(wstring&)
{
	wstring wordPath = ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage);
	vector<wstring> translations;
	getVectorFromWfile(wordPath, translations);

	while (!translations.empty()) { deleteTranslationManual(*currentWordPointer, wordPath, translations, translations.front()); }
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
	wstring wordPath = ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage);
	vector<wstring> translations;
	getVectorFromWfile(wordPath, translations);
	addTranslationsManual(*currentWordPointer, wordPath, translations);
}

void translationEditorFunction(wstring& optionName)
{
	wstring wordPath = ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage);
	vector<wstring> translations;

	getVectorFromWfile(wordPath, translations);
	if (translations.size() == 0)
	{
		wcout << "Translations not found! The word will be deleted." << endl;
		_wsystem(L"pause");
		_wremove(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage).c_str());
		return;
	}

	if (contains(translations, *currentTranslationPointer))
	{
		if (optionName == L"Edit")
		{
			vector<wstring> reverseTranslations;
			wstring reverseWordPath;
			wstring oldTranslation = wstring(*currentTranslationPointer);
			removeTextInBracket(oldTranslation);

			reverseWordPath = ProgramDirectories::getPathToFile(oldTranslation, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);
			getVectorFromWfile(reverseWordPath, reverseTranslations);
			if (!reverseTranslations.empty())
			{
				vector<wstring>::iterator it = find(reverseTranslations.begin(), reverseTranslations.end(), *currentWordPointer);
				reverseTranslations.erase(it);

				if (!reverseTranslations.empty()) { saveVectorToWfile(reverseWordPath, reverseTranslations); }
				else { _wremove(reverseWordPath.c_str()); }

				reverseTranslations.clear();
			}
			reverseWordPath.clear();

			vector<wstring>::iterator currentTranslateIt = find(translations.begin(), translations.end(), *currentTranslationPointer);
			wconsoleMenu::consoleWstringEditor(*currentTranslationPointer);
			*currentTranslateIt = *currentTranslationPointer;

			if (!contains(translations, *currentTranslationPointer))
			{
				wstring newTranslation = wstring(*currentTranslationPointer);
				removeTextInBracket(newTranslation);

				reverseWordPath = ProgramDirectories::getPathToFile(newTranslation, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);
				getVectorFromWfile(reverseWordPath, reverseTranslations);

				if (!contains(reverseTranslations, *currentWordPointer))
				{
					reverseTranslations.push_back(*currentWordPointer);
					saveVectorToWfile(reverseWordPath, reverseTranslations);
				}
			}
			else { translations.erase(currentTranslateIt); }
		}

		if (optionName == L"Delete")
		{
			deleteTranslationManual(*currentWordPointer, wordPath, translations, *currentTranslationPointer);

			if (translations.size() == 0)
			{
				wcout << L"You have removed all existed translations. The word will be deleted." << endl;
				_wsystem(L"pause");
				_wremove(wordPath.c_str());
				return;
			}
		}

		saveVectorToWfile(wordPath, translations);
	}
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
	wstring oldWord = wstring(*currentWordPointer);
	wstring oldWordPath = ProgramDirectories::getPathToFile(oldWord, currentLanguage, currentStage);
	vector<wstring> translations;
	getVectorFromWfile(oldWordPath, translations);

	_wremove(oldWordPath.c_str());
	wconsoleMenu::consoleWstringEditor(*currentWordPointer);

	wstring newWord = wstring(*currentWordPointer);
	wstring newWordPath = ProgramDirectories::getPathToFile(newWord, currentLanguage, currentStage);
	saveVectorToWfile(newWordPath, translations);

	for (wstring translation : translations)
	{
		removeTextInBracket(translation);

		wstring reverseWordPath = ProgramDirectories::getPathToFile(translation, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);
		vector<wstring> reverseTranslations;
		vector<wstring>::iterator translationIt;

		getVectorFromWfile(reverseWordPath, reverseTranslations);
		if (contains(reverseTranslations, oldWord))
		{
			translationIt = find(reverseTranslations.begin(), reverseTranslations.end(), oldWord);
			*translationIt = newWord;
		}
		else { reverseTranslations.push_back(newWord); }
		saveVectorToWfile(reverseWordPath, reverseTranslations);
	}
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
		vector<wstring> options;
		vector<void (*)(wstring&)> functions;
		wconsoleMenu languageOfWord;

		if (converterFunction)
		{
			options = { L"Edit saved words" , L"Add new word" , L"Get several random words from learned" ,
				(L"Convert words from " + ProgramDirectories::reverseLanguage(currentLanguage)) };
			functions = { displayAllSavedWordsToEdit , addWordFunction , getRandomWordsFromLearned , convertWordsFromAnotherLanguage };
		}
		else
		{
			options = { L"Edit saved words" , L"Add new word" , L"Get several random words from learned" };
			functions = { displayAllSavedWordsToEdit , addWordFunction , getRandomWordsFromLearned };
		}

		languageOfWord = wconsoleMenu(options, functions, L"What do you want to do?", L"I changed my mind. Back, please");
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