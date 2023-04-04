#include "ManageWordsProcess.h"

wstring currentLanguage;
wstring currentStage;
wstring* currentWordPointer;
wstring* currentTranslationPointer;
const size_t MAX_WORD_LENGTH = 255;

void convertWordsFromAnotherLanguage(wstring language)
{
	set<pair<wstring, wstring>> correspondences;
	addAllPairsCorrespondencesToSetFrom(ProgramDirectories::getPathToDirectory(language, ProgramDirectories::stages.unlearned), correspondences);
	addAllPairsCorrespondencesToSetFrom(ProgramDirectories::getPathToDirectory(ProgramDirectories::reverseLanguage(language), ProgramDirectories::stages.unlearned), correspondences, true);
	addAllPairsCorrespondencesToSetFrom(ProgramDirectories::getPathToDirectory(ProgramDirectories::reverseLanguage(language), ProgramDirectories::stages.learned), correspondences, true);

	if (!correspondences.empty())
	{
		wstring currentWord = correspondences.begin()->first;
		vector<wstring> translations;

		for (set<pair<wstring, wstring>>::iterator correspondenceIt = correspondences.begin(); correspondenceIt != correspondences.end(); correspondenceIt++)
		{
			if (correspondenceIt->first != currentWord)
			{
				saveVectorToWfile(ProgramDirectories::getPathToFile(currentWord, language, ProgramDirectories::stages.unlearned), translations);
				currentWord = correspondenceIt->first;
				translations.clear();
			}

			translations.push_back(correspondenceIt->second);
		}
		saveVectorToWfile(ProgramDirectories::getPathToFile(currentWord, language, ProgramDirectories::stages.unlearned), translations);
	}
}

void getRandomWordsFromLearned(wstring&)
{
	int amount;
	vector<wstring> wordsToMove;

	getWords(ProgramDirectories::getPathToDirectory(currentLanguage, ProgramDirectories::stages.learned), wordsToMove);
	if (wordsToMove.size() > 0)
	{
		wcout << L"Enter an amount of words from 1 to " << wordsToMove.size() << L": ";
		if (wcin >> amount)
		{
			wcin.ignore(LLONG_MAX, '\n');

			if (amount <= 0)
			{
				_wsystem(L"cls");
				wcout << L"An invalid operation." << endl;
				_wsystem(L"pause");
				return;
			}

			if (amount > wordsToMove.size())
			{
				_wsystem(L"cls");
				wcout << L"Too much. All words will be moved." << endl;
				_wsystem(L"pause");

				amount = wordsToMove.size();
			}

			if (amount <= (wordsToMove.size() + 1) / 2)
			{
				for (int i = 0; i < amount; i++)
				{
					wstring wordToMove = randomWstring(wordsToMove);

					if (!MoveFileW(ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.learned).c_str(),
						ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.unlearned).c_str()))
					{
						wcout << L"The word \"" << wordToMove << L"\" didn't moved" << endl;
						_wsystem(L"pause");
					}

					removeElement(wordToMove, wordsToMove);
				}
			}
			else
			{
				for (int i = 0; i < wordsToMove.size() - amount; i++)
				{
					wstring remainingWord = randomWstring(wordsToMove);
					removeElement(remainingWord, wordsToMove);
				}

				for (wstring wordToMove: wordsToMove)
				{
					if (!MoveFileW(ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.learned).c_str(),
						ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.unlearned).c_str()))
					{
						wcout << L"The word \"" << wordToMove << L"\" didn't moved" << endl;
						_wsystem(L"pause");
					}
				}
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
}

void addTranslationsManual(wstring& word, wstring& wordPath, vector<wstring>& translations)
{
	_wsystem(L"cls");

	vector<wstring> translationsWithoutBrackets;
	for (wstring translation : translations)
	{
		removeTextInBracket(translation);
		translationsWithoutBrackets.push_back(translation);
	}

	while (true)
	{
		wstring translation;
		wstring translationWhithoutBrackets;

		wcout << L"Leave empty to exit." << endl;
		wcout << L"Enter a translation: ";

		getline(wcin, translation);
		if (translation.empty())
		{
			_wsystem(L"cls");
			break;
		}

		wordToLowerCase(translation);
		translationWhithoutBrackets = wstring(translation);
		removeTextInBracket(translationWhithoutBrackets);

		if (!contains(translationsWithoutBrackets, translationWhithoutBrackets))
		{
			translationsWithoutBrackets.push_back(translationWhithoutBrackets);
			translations.push_back(translation);

			if (translationWhithoutBrackets.size() <= MAX_WORD_LENGTH)
			{
				wstring reverseWordPath;
				vector<wstring> reverseTranslations;
				vector<wstring> reverseTranslationsWithoutBrackets;

				reverseWordPath = ProgramDirectories::getPathToFile(translationWhithoutBrackets, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.learned);
				if (!isPathExist(reverseWordPath))
				{
					reverseWordPath = ProgramDirectories::getPathToFile(translationWhithoutBrackets, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);
				}

				getVectorFromWfile(reverseWordPath, reverseTranslations);
				getVectorFromWfile(reverseWordPath, reverseTranslationsWithoutBrackets, true);

				if (!contains(reverseTranslationsWithoutBrackets, word))
				{
					wstring question = L"Add translation \"" + word + L"\" into the word \"" + translationWhithoutBrackets + L"\"?";
					if (askQuestion(question))
					{
						reverseTranslations.push_back(word);
						saveVectorToWfile(reverseWordPath, reverseTranslations);
					}
				}
			}
		}
		else
		{
			wcout << L"Already exist." << endl;
			_wsystem(L"pause");
		}

		_wsystem(L"cls");
	}

	if (translations.size() != 0) { saveVectorToWfile(wordPath, translations); }
	else
	{
		wcout << L"There is no one translation. Nothing added" << endl;
		_wsystem(L"pause");
	}
}

void addWordFunction(wstring&)
{
	wstring word;
	wstring wordPath;
	vector<wstring> translations;

	wcout << L"Enter the word: ";
	getline(wcin, word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}

	if (word.length() > MAX_WORD_LENGTH)
	{
		wcout << L"Too long word" << endl;
		_wsystem(L"pause");
		return;
	}

	wordToLowerCase(word);

	wordPath = ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.unlearned);
	if (isPathExist(wordPath))
	{
		getVectorFromWfile(wordPath, translations);

		wcout << L"Word \"" << word << L'\"';
		wcout << L" with translations: { ";
		printVector(translations, L", ", false);
		wcout << L" } already exist." << endl;
		_wsystem(L"pause");

		if (!askQuestion(L"Would you like to add new translations?")) return;
	}
	else if (isPathExist(ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::reverseStage(currentStage))))
	{
		if (askQuestion(L"This word already in learned folder. Would you like to return back into unlearned?"))
		{
			MoveFileW(ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.learned).c_str(), wordPath.c_str());
			getVectorFromWfile(wordPath, translations);

			wcout << L"Word \"" << word << L'\"';
			wcout << L" with translations: { ";
			printVector(translations, L", ", false);
			wcout << L"} moved to unlearned." << endl;
			_wsystem(L"pause");

			if (!askQuestion(L"Would you like to add new translations?")) return;
		}
	}

	addTranslationsManual(word, wordPath, translations);
}

void deleteTranslationManual(wstring& word, wstring& wordPath, vector<wstring>& translations, wstring& translationToDelete)
{
	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		vector<wstring> reverseTranslations;
		wstring reverseWordPath = ProgramDirectories::getPathToFile(translationToDelete, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);

		getVectorFromWfile(reverseWordPath, reverseTranslations, true);
		if (contains(reverseTranslations, word))
		{
			wstring question = L"Delete the translation \"" + word + L"\" from the word's \"" + translationToDelete + L"\" translations?";
			if (askQuestion(question))
			{
				removeElement(word, reverseTranslations);
				if (!reverseTranslations.empty()) { saveVectorToWfile(reverseWordPath, reverseTranslations); }
				else
				{
					wcout << L"Translations for the word \"" << translationToDelete << L"\" didn't remain. It will be deleted." << endl;
					_wsystem(L"pause");
					_wremove(reverseWordPath.c_str());
					ProgramDirectories::removeWordFromProgramFiles(translationToDelete, ProgramDirectories::reverseLanguage(currentLanguage));
				}
			}
		}
	}

	removeElement(translationToDelete, translations);
	_wsystem(L"cls");
}

void deleteWordFunction(wstring&)
{
	wstring wordPath = ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage);
	vector<wstring> translations;
	getVectorFromWfile(wordPath, translations, true);

	while (!translations.empty()) { deleteTranslationManual(*currentWordPointer, wordPath, translations, translations.front()); }
	_wremove(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage).c_str());
	if (currentStage == ProgramDirectories::stages.unlearned) { ProgramDirectories::removeWordFromProgramFiles(*currentWordPointer, currentLanguage); }

	wcout << "Done!" << endl;
	_wsystem(L"pause");
}

void moveToUnlearned(wstring&)
{
	MoveFileW(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, ProgramDirectories::stages.learned).c_str(),
		ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, ProgramDirectories::stages.unlearned).c_str());

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
			bool didWordExistInTranslation = false;

			vector<wstring>::iterator currentTranslateIt = find(translations.begin(), translations.end(), *currentTranslationPointer);

			_wsystem(L"cls");
			wcout << L"Edit: ";
			wconsoleMenu::consoleWstringEditor(*currentTranslationPointer);

			if (*currentTranslationPointer == oldTranslation) { return; }
			if (currentTranslationPointer->empty())
			{
				wcout << L"Translation can not be empty" << endl;
				_wsystem(L"pause");

				*currentTranslationPointer = oldTranslation;
				return;
			}

			translations.erase(currentTranslateIt);

			removeTextInBracket(oldTranslation);
			reverseWordPath = ProgramDirectories::getPathToFile(oldTranslation, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);
			getVectorFromWfile(reverseWordPath, reverseTranslations, true);
			if (!reverseTranslations.empty() && contains(reverseTranslations, *currentWordPointer))
			{
				didWordExistInTranslation = true;
				removeElement(*currentWordPointer, reverseTranslations);

				if (!reverseTranslations.empty()) { saveVectorToWfile(reverseWordPath, reverseTranslations); }
				else
				{
					_wremove(reverseWordPath.c_str());
					if (currentStage == ProgramDirectories::stages.unlearned)
					{
						ProgramDirectories::removeWordFromProgramFiles(oldTranslation, ProgramDirectories::reverseLanguage(currentLanguage));
					}
				}

				reverseTranslations.clear();
			}
			reverseWordPath.clear();

			wstring newTranslation = wstring(*currentTranslationPointer);
			wstring newTranslationWithoutBracket = wstring(*currentTranslationPointer);
			removeTextInBracket(newTranslationWithoutBracket);

			if (!contains(translations, newTranslation))
			{
				translations.insert(currentTranslateIt , newTranslation);
				saveVectorToWfile(wordPath, translations);

				if (didWordExistInTranslation && newTranslationWithoutBracket.length() < MAX_WORD_LENGTH)
				{
					reverseWordPath = ProgramDirectories::getPathToFile(newTranslationWithoutBracket, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);
					getVectorFromWfile(reverseWordPath, reverseTranslations, true);

					if (!contains(reverseTranslations, *currentWordPointer))
					{
						reverseTranslations.push_back(*currentWordPointer);
						saveVectorToWfile(reverseWordPath, reverseTranslations);
					}
				}
			}
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
	vector<wstring> editorOptions;
	vector<void (*)(wstring&)> functions;
	wstring selectText = L"Select an action";
	wstring exitText = L"Back";

	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		editorOptions = { L"Edit" , L"Delete" };
		functions = functionMultiplier(translationEditorFunction, editorOptions.size());
	}
	else
	{
		editorOptions = { L"Delete" };
		functions = { translationEditorFunction };
	}

	wconsoleMenu languageOfWord(editorOptions, functions, selectText, exitText);

	currentTranslationPointer = &translation;
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
		wstring selectText;
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
			selectText = L"Filter translations by characters: ";
			displayTranslations = wconsoleMenu(translations, functions, selectText);

			displayTranslations.singleSelectWithFilter(optionNum, filterText, skipFilter);
			skipFilter = true;
		}
		else
		{
			selectText = L"Select a translation";
			displayTranslations = wconsoleMenu(translations, functions, selectText);

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

	_wsystem(L"cls");
	wcout << L"Edit: ";
	wconsoleMenu::consoleWstringEditor(*currentWordPointer);

	if (*currentWordPointer == oldWord) { return; }
	if (currentWordPointer->empty())
	{
		wcout << L"Word can not be empty" << endl;
		_wsystem(L"pause");

		*currentWordPointer = oldWord;
		return;
	}
	if (currentWordPointer->length() > MAX_WORD_LENGTH)
	{
		wcout << L"Word too long" << endl;
		_wsystem(L"pause");

		*currentWordPointer = oldWord;
		return;
	}

	_wremove(oldWordPath.c_str());
	if (currentStage == ProgramDirectories::stages.unlearned) { ProgramDirectories::removeWordFromProgramFiles(oldWord, currentLanguage); }

	wstring newWord = wstring(*currentWordPointer);
	wstring newWordPath = ProgramDirectories::getPathToFile(newWord, currentLanguage, currentStage);
	saveVectorToWfile(newWordPath, translations);

	for (wstring translation : translations)
	{
		removeTextInBracket(translation);

		wstring reverseWordPath = ProgramDirectories::getPathToFile(translation, ProgramDirectories::reverseLanguage(currentLanguage), ProgramDirectories::stages.unlearned);
		vector<wstring> reverseTranslations;
		vector<wstring>::iterator translationIt;
		bool doOldWordExistInTranslation = false;

		getVectorFromWfile(reverseWordPath, reverseTranslations);
		for (int i = 0; i < reverseTranslations.size(); i++)
		{
			size_t bracketPosition = reverseTranslations[i].find(L'(');
			wstring bracketWithText;
			if (bracketPosition != string::npos)
			{
				bracketWithText = L' ' + reverseTranslations[i].substr(bracketPosition);
				removeTextInBracket(reverseTranslations[i]);
			}
			else { bracketWithText = L""; }

			if (reverseTranslations[i] == oldWord)
			{
				doOldWordExistInTranslation = true;
				reverseTranslations[i] = (newWord + bracketWithText);
				saveVectorToWfile(reverseWordPath, reverseTranslations);
				break;
			}
		}

		if (!doOldWordExistInTranslation)
		{
			wstring question = L"The translation \"" + oldWord + L"\" didn't found in the word \"" + translation
				+ L"\". Add the translation \"" + newWord + L"\" into the word \"" + translation + L"\"?";
			if (askQuestion(question))
			{
				reverseTranslations.push_back(newWord);
				saveVectorToWfile(reverseWordPath, reverseTranslations);
			}
		}
	}
}

void wordEditor(wstring& word)
{
	vector<wstring> editorOptions;
	vector<void (*)(wstring&)> functions;
	wstring selectText = L"Select an action";
	wstring exitText = L"Back";
	wconsoleMenu editor;
	short optionNum = 0;

	currentWordPointer = &word;

	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		editorOptions = { L"Rename" , L"View translations" , L"Add translations" , L"Delete" };
		functions = { ranameWordFunction , displayTranslations , addTranslationFunction , deleteWordFunction };
		editor = wconsoleMenu(editorOptions, functions, selectText, exitText);

		do
		{
			editor.singleSelect(optionNum);
		} while (optionNum != -1 && optionNum != 3);
	}
	else
	{
		editorOptions = { L"View translations" , L"Move to unlearned", L"Delete" };
		functions = { displayTranslations , moveToUnlearned , deleteWordFunction };
		editor = wconsoleMenu(editorOptions, functions, selectText, exitText);

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
			selectText = L"Select a word:";
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
		vector<wstring> options = { L"Edit saved words" , L"Add new word" , L"Get several random words from \"learned\"" };
		vector<void (*)(wstring&)> functions = { displayAllSavedWordsToEdit , addWordFunction , getRandomWordsFromLearned };
		wstring selectText = L"Select an action";
		wstring exitText = L"Back";
		wconsoleMenu languageOfWord = wconsoleMenu(options, functions, selectText, exitText);

		languageOfWord.cyclicSelect();
	}
	else { displayAllSavedWordsToEdit(currentStage); }

	currentStage.clear();
}

void wordsStageToManage(wstring& language)
{
	vector<wstring> stageOfWords = { ProgramDirectories::stages.unlearned , ProgramDirectories::stages.learned };
	vector<void (*)(wstring&)> functions = functionMultiplier(addNewOrEdit, stageOfWords.size());
	wstring selectText = L"Select a stage";
	wstring exitText = L"Back";
	wconsoleMenu editWordsMenu(stageOfWords, functions, selectText, exitText);

	currentLanguage = language;
	editWordsMenu.cyclicSelect();
	currentLanguage.clear();
}

void wordsLanguageToManage(wstring&)
{
	vector<wstring> languages = { ProgramDirectories::languages.native , ProgramDirectories::languages.target };
	vector<void (*)(wstring&)> functions = functionMultiplier(wordsStageToManage, languages.size());
	wstring selectText = L"Select a language";
	wstring exitText = L"Back";
	wconsoleMenu languageOfWord(languages, functions, selectText, exitText);

	languageOfWord.cyclicSelect();
}