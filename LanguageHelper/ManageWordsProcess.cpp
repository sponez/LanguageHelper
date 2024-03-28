#include "ManageWordsProcess.h"

wstring currentLanguage;
wstring currentStage;
wstring* currentWordPointer;
wstring* currentTranslationPointer;
const size_t MAX_WORD_LENGTH = 255;

wstring findWordPath(wstring word, wstring language) {
	wstring wordPath;
	
	wordPath = ProgramDirectories::getPathToFile(
		word,
		language,
		ProgramDirectories::stages.learned
	);

	if (!isPathExist(wordPath))
	{
		wordPath = ProgramDirectories::getPathToFile(
			word,
			language,
			ProgramDirectories::stages.unlearned
		);
	}

	return wordPath;
}

void getRandomWordsFromLearned(wstring&)
{
	int amount;
	vector<wstring> wordsToMove;

	getEntries(ProgramDirectories::getPathToDirectory(currentLanguage, ProgramDirectories::stages.learned), wordsToMove);
	if (wordsToMove.size() > 0) {
		wcout << L"Enter an amount of words from 1 to " << wordsToMove.size() << L": ";

		if (wcin >> amount) {
			wcin.ignore(LLONG_MAX, L'\n');

			if (amount <= 0) {
				_wsystem(L"cls");
				wcout << L"An invalid operation." << L'\n';
				_wsystem(L"pause");
				return;
			}

			if (amount > wordsToMove.size()) {
				_wsystem(L"cls");
				wcout << L"Too much. All words will be moved." << L'\n';
				_wsystem(L"pause");

				amount = wordsToMove.size();
			}

			if (amount <= (wordsToMove.size() + 1) / 2) {
				for (int i = 0; i < amount; ++i) {
					wstring wordToMove = randomWstringFromArray(wordsToMove);

					if (
						!MoveFileW(
							ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.learned).c_str(),
							ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.unlearned).c_str()
						)
					) {
						wcout << L"The word \"" << wordToMove << L"\" didn't moved" << L'\n';
						_wsystem(L"pause");
					}

					removeElement(wordToMove, wordsToMove);
				}
			}
			else {
				for (int i = 0; i < wordsToMove.size() - amount; ++i) {
					wstring remainingWord = randomWstringFromArray(wordsToMove);
					removeElement(remainingWord, wordsToMove);
				}

				for (wstring wordToMove: wordsToMove) {
					if (
						!MoveFileW(
							ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.learned).c_str(),
							ProgramDirectories::getPathToFile(wordToMove, currentLanguage, ProgramDirectories::stages.unlearned).c_str()
						)
					) {
						wcout << L"The word \"" << wordToMove << L"\" didn't moved" << L'\n';
						_wsystem(L"pause");
					}
				}
			}

			_wsystem(L"cls");
			wcout << L"Done!" << L'\n';
			_wsystem(L"pause");
		}
		else
		{
			wcin.ignore(LLONG_MAX, '\n');

			_wsystem(L"cls");
			wcout << L"Something went wrong." << L'\n';
			_wsystem(L"pause");
			return;
		}
	}
}

void addTranslationsManual(wstring& word, wstring& wordPath, vector<wstring>& translations)
{
	_wsystem(L"cls");

	vector<wstring> translationsWithoutBrackets;
	for (wstring translation : translations) {
		cutTextInBracket(translation);
		translationsWithoutBrackets.push_back(translation);
	}

	while (true) {
		wstring translation;
		wstring textInBrackets;

		wcout << L"Leave empty to exit." << L'\n';
		wcout << L"Enter a translation: ";

		getline(wcin, translation);
		if (translation.empty())
		{
			_wsystem(L"cls");
			break;
		}

		wordToLowerCase(translation);
		textInBrackets = cutTextInBracket(translation);

		if (!contains(translationsWithoutBrackets, translation)) {
			translationsWithoutBrackets.push_back(translation);
			translations.push_back(standartTranslationForm(translation, textInBrackets));

			if (translation.length() <= MAX_WORD_LENGTH) {
				wstring reverseWordPath;
				vector<wstring> reverseTranslations;
				vector<wstring> reverseTranslationsWithoutBrackets;

				reverseWordPath = findWordPath(translation, ProgramDirectories::reverseLanguage(currentLanguage));
				getVectorFromWfile(reverseWordPath, reverseTranslations);
				getVectorFromWfile(reverseWordPath, reverseTranslationsWithoutBrackets, true);

				if (!contains(reverseTranslationsWithoutBrackets, word)) {
					wstring question = L"Add translation \"" + word + L"\" into the word \"" + translation + L"\"?";

					if (askQuestion(question)) {
						wstring newReverseTranslation;

						if (textInBrackets.length() > 0 && askQuestion(L"Add brackets with text \"" + textInBrackets + L"\"?")) {
							newReverseTranslation = standartTranslationForm(word, textInBrackets);
						}
						else {
							newReverseTranslation = wstring(word);
						}

						reverseTranslations.push_back(newReverseTranslation);
						sort(reverseTranslations.begin(), reverseTranslations.end());
						saveVectorToWfile(reverseWordPath, reverseTranslations);
					}
				}
			}
		}
		else {
			wcout << L"Already exist." << L'\n';
			_wsystem(L"pause");
		}

		_wsystem(L"cls");
	}

	if (translations.size() != 0) {
		sort(translations.begin(), translations.end());
		saveVectorToWfile(wordPath, translations);
	}
	else {
		wcout << L"There is no one translation. Nothing added" << L'\n';
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

	if (word.length() == 0) {
		wcout << L"Empty word" << L'\n';
		_wsystem(L"pause");
		return;
	}

	if (word.length() > MAX_WORD_LENGTH) {
		wcout << L"Too long word" << L'\n';
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
		wcout << L" } already exist." << L'\n';
		_wsystem(L"pause");

		if (!askQuestion(L"Would you like to add new translations?")) {
			return;
		}
	}
	else if (isPathExist(ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::reverseStage(currentStage))))
	{
		if (askQuestion(L"This word already in learned folder. Would you like to return back into unlearned?")) {
			MoveFileW(ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.learned).c_str(), wordPath.c_str());
			getVectorFromWfile(wordPath, translations);

			wcout << L"Word \"" << word << L'\"';
			wcout << L" with translations: { ";
			printVector(translations, L", ", false);
			wcout << L" } moved to unlearned." << L'\n';
			_wsystem(L"pause");

			if (!askQuestion(L"Would you like to add new translations?")) {
				return;
			}
		}
	}

	addTranslationsManual(word, wordPath, translations);
}

void deleteTranslationManual(wstring& word, wstring& wordPath, vector<wstring>& translations, wstring& translationToDelete)
{
	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		wstring translationToDeleteWithoutBrackets;
		wstring reverseWordPath;
		vector<wstring> reverseTranslations;

		translationToDeleteWithoutBrackets = wstring(translationToDelete);
		cutTextInBracket(translationToDeleteWithoutBrackets);
		reverseWordPath = findWordPath(translationToDeleteWithoutBrackets, ProgramDirectories::reverseLanguage(currentLanguage));
		getVectorFromWfile(reverseWordPath, reverseTranslations, true);

		if (contains(reverseTranslations, word))
		{
			wstring question = L"Delete the translation \"" + word + L"\" from the word's \"" + translationToDeleteWithoutBrackets + L"\" translations?";
			if (askQuestion(question))
			{
				removeElement(word, reverseTranslations);
				if (!reverseTranslations.empty())
				{
					saveVectorToWfile(reverseWordPath, reverseTranslations);
				}
				else
				{
					wcout << L"Translations for the word \"" << translationToDeleteWithoutBrackets << L"\" didn't remain. It will be deleted." << L'\n';
					_wsystem(L"pause");
					_wremove(reverseWordPath.c_str());
					ProgramDirectories::removeWordFromProgramFiles(translationToDeleteWithoutBrackets, ProgramDirectories::reverseLanguage(currentLanguage));
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
	getVectorFromWfile(wordPath, translations);

	while (!translations.empty())
	{
		deleteTranslationManual(
			*currentWordPointer,
			wordPath,
			translations,
			translations.front()
		);
	}

	_wremove(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage).c_str());
	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		ProgramDirectories::removeWordFromProgramFiles(*currentWordPointer, currentLanguage);
	}

	wcout << "Done!" << L'\n';
	_wsystem(L"pause");
}

void moveToUnlearned(wstring&)
{
	MoveFileW(
		ProgramDirectories::getPathToFile(
			*currentWordPointer,
			currentLanguage,
			ProgramDirectories::stages.learned
		).c_str(),

		ProgramDirectories::getPathToFile(
			*currentWordPointer,
			currentLanguage,
			ProgramDirectories::stages.unlearned
		).c_str()
	);

	wcout << "Done!" << L'\n';
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
		wcout << "Translations not found! The word will be deleted." << L'\n';
		_wsystem(L"pause");
		_wremove(ProgramDirectories::getPathToFile(*currentWordPointer, currentLanguage, currentStage).c_str());
		return;
	}

	if (contains(translations, *currentTranslationPointer))
	{
		if (optionName == L"Edit")
		{
			vector<wstring> reverseTranslations;
			vector<wstring> reverseTranslationsWithoutBrackets;
			wstring reverseWordPath;

			wstring oldTranslation = wstring(*currentTranslationPointer);
			wstring oldTextInBracket = cutTextInBracket(oldTranslation);

			bool didWordExistInTranslation = false;

			vector<wstring>::iterator currentTranslateIt = find(translations.begin(), translations.end(), *currentTranslationPointer);

			_wsystem(L"cls");
			wcout << L"Edit: ";
			wconsoleMenu::consoleWstringEditor(*currentTranslationPointer);

			wstring newTranslation = wstring(*currentTranslationPointer);
			wordToLowerCase(newTranslation);

			wstring newTextInBracket = cutTextInBracket(newTranslation);

			if (newTranslation == oldTranslation && newTextInBracket == oldTextInBracket)
			{
				*currentTranslationPointer = standartTranslationForm(oldTranslation, oldTextInBracket);
				return;
			}

			if (newTranslation.empty())
			{
				wcout << L"Translation can not be empty" << L'\n';
				_wsystem(L"pause");

				*currentTranslationPointer = standartTranslationForm(oldTranslation, oldTextInBracket);
				return;
			}

			translations.erase(currentTranslateIt);

			reverseWordPath = findWordPath(oldTranslation, ProgramDirectories::reverseLanguage(currentLanguage));
			getVectorFromWfile(reverseWordPath, reverseTranslations);
			getVectorFromWfile(reverseWordPath, reverseTranslationsWithoutBrackets, true);

			if (!reverseTranslationsWithoutBrackets.empty() && contains(reverseTranslationsWithoutBrackets, *currentWordPointer))
			{
				wstring oldReverseTranslation = standartTranslationForm(*currentWordPointer, oldTextInBracket);

				didWordExistInTranslation = true;
				removeElement(oldReverseTranslation, reverseTranslations);

				if (!reverseTranslations.empty())
				{
					saveVectorToWfile(reverseWordPath, reverseTranslations);
				}
				else
				{
					_wremove(reverseWordPath.c_str());
					if (currentStage == ProgramDirectories::stages.unlearned)
					{
						ProgramDirectories::removeWordFromProgramFiles(
							oldTranslation,
							ProgramDirectories::reverseLanguage(currentLanguage)
						);
					}
				}

				reverseTranslations.clear();
				reverseTranslationsWithoutBrackets.clear();
			}
			reverseWordPath.clear();

			if (!contains(translations, newTranslation))
			{
				translations.insert(currentTranslateIt, standartTranslationForm(newTranslation, newTextInBracket));
				saveVectorToWfile(wordPath, translations);

				if (didWordExistInTranslation && newTranslation.length() < MAX_WORD_LENGTH)
				{
					reverseWordPath = findWordPath(newTranslation, ProgramDirectories::reverseLanguage(currentLanguage));
					getVectorFromWfile(reverseWordPath, reverseTranslations);
					getVectorFromWfile(reverseWordPath, reverseTranslationsWithoutBrackets, true);

					if (!contains(reverseTranslationsWithoutBrackets, *currentWordPointer))
					{
						wstring newReverseTranslation = standartTranslationForm(*currentWordPointer, newTextInBracket);

						reverseTranslations.push_back(newReverseTranslation);
						saveVectorToWfile(reverseWordPath, reverseTranslations);
					}
					else
					{
						for (int i = 0; i < reverseTranslations.size(); ++i)
						{
							wstring reverseTranslation = wstring(reverseTranslations[i]);
							wstring textInBracketsOfReverseTranslation = cutTextInBracket(reverseTranslation);

							if (reverseTranslation == *currentWordPointer)
							{
								if (textInBracketsOfReverseTranslation != newTextInBracket)
								{
									wstring question =
										L"Translation \""
										+ reverseTranslation
										+ L"\" already exist in word \""
										+ newTranslation
										+ L"\", but commentary is different. Would you like to change it?";

									if (askQuestion(question))
									{
										reverseTranslations[i] = standartTranslationForm(*currentWordPointer, newTextInBracket);
										saveVectorToWfile(reverseWordPath, reverseTranslations);
									}

									break;
								}
								else
								{
									break;
								}
							}
						}
					}
				}
			}
		}

		if (optionName == L"Delete")
		{
			deleteTranslationManual(*currentWordPointer, wordPath, translations, *currentTranslationPointer);

			if (translations.size() == 0)
			{
				wcout << L"You have removed all existed translations. The word will be deleted." << L'\n';
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
			wcout << "Translations not found! The word will be deleted." << L'\n';
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
		wcout << L"Word can not be empty" << L'\n';
		_wsystem(L"pause");

		*currentWordPointer = oldWord;
		return;
	}
	if (currentWordPointer->length() > MAX_WORD_LENGTH)
	{
		wcout << L"Word too long" << L'\n';
		_wsystem(L"pause");

		*currentWordPointer = oldWord;
		return;
	}
	if (
		isPathExist(
			ProgramDirectories::getPathToFile(
				*currentWordPointer,
				currentLanguage,
				ProgramDirectories::stages.learned
			)
		) ||
		isPathExist(
			ProgramDirectories::getPathToFile(
				*currentWordPointer,
				currentLanguage,
				ProgramDirectories::stages.unlearned
			)
		)
	) {
		wcout << L"Word already exist" << L'\n';
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
		cutTextInBracket(translation);

		wstring reverseWordPath = findWordPath(translation, ProgramDirectories::reverseLanguage(currentLanguage));
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
				cutTextInBracket(reverseTranslations[i]);
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
		editor.setMainMenuFlag();

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
		editor.setMainMenuFlag();

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

		getEntries(ProgramDirectories::getPathToDirectory(currentLanguage, currentStage), words);
		if (words.size() == 0)
		{
			wcout << "Words not found!" << L'\n';
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

		languageOfWord.setMainMenuFlag();
		languageOfWord.cyclicSelect();
	}
	else {
		displayAllSavedWordsToEdit(currentStage);
	}

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