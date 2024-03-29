#include "TestProcess.h"
#include "ViewProcess.h"

wstring currentLanguage;
wstring currentStage;

double findTimeToAnswer(wstring& word, vector<wstring>& translations)
{
	if (ProgramDirectories::programProperties.millisecondsToAnswerForCharacter.value != INT_MAX)
	{
		const double TIME_TO_THINK = 3000;
		unsigned long long maxLen = 0;

		for (wstring translation : translations) {
			wstring translationWithoutBrackets = wstring(translation);
			cutTextInBracket(translationWithoutBrackets);

			maxLen = max(maxLen, translationWithoutBrackets.length());
		}

		return TIME_TO_THINK + maxLen * ProgramDirectories::programProperties.millisecondsToAnswerForCharacter.value;
	}

	return INT_MAX;
}

void openAnswerTest(wstring&)
{
	vector<wstring> allWords;

	getEntries(
		ProgramDirectories::getPathToDirectory(currentLanguage, currentStage), 
		allWords
	);

	if (allWords.empty()) {
		std::wcout << L"Words not found!" << L'\n';
		_wsystem(L"pause");
		_wsystem(L"cls");
		return;
	}

	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		int amountOfRepeats = 0;
		int correctAnswers = 0;
		vector<wstring> usedWords;
		vector<wstring> unusedWords;
		map<wstring, int> overallCorrectAnswers;

		getMapFromWfile(
			ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.successSave, currentLanguage),
			overallCorrectAnswers
		);

		getVectorFromWfile(
			ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.usedWords, currentLanguage),
			usedWords
		);

		vectorDifference(allWords, usedWords, unusedWords);
		int wordsToUse = unusedWords.size();

		for (int i = usedWords.size();; ++i) {
			if (i >= allWords.size()) {
				unusedWords = vector<wstring>(usedWords);
				usedWords.clear();

				if (ProgramDirectories::programProperties.endlessTesting.value) {
					i = 0;
				}
				else {
					std::wcout << L"You have passed all words! ";

					if (ProgramDirectories::programProperties.randomTesting.value) {
						std::wcout << L"Start again to shuffle the words." << L'\n';
					}
					else {
						std::wcout << L"Start again to return to the first word." << L'\n';
					}

					_wsystem(L"pause");
					_wsystem(L"cls");
					break;
				}
			}

			wstring word;
			if (unusedWords.size() > 0) {
				if (ProgramDirectories::programProperties.randomTesting.value) {
					word = randomWstringFromArray(unusedWords);
				}
				else {
					word = unusedWords[0];
				}
			}
			else {
				std::wcout << L"Words are run out!" << L'\n';
				_wsystem(L"pause");
				_wsystem(L"cls");
				break;
			}

			vector<wstring> translations;
			getVectorFromWfile(
				ProgramDirectories::getPathToFile(word, currentLanguage, currentStage), 
				translations
			);

			if (translations.empty()) {
				std::wcout << L"Word doesn't exist already" << L'\n';
				_wsystem(L"pause");

				removeElement(word, usedWords);
				removeElement(word, unusedWords);
				overallCorrectAnswers.erase(word);
				continue;
			}

			if (ProgramDirectories::programProperties.endlessTesting.value) {
				std::wcout << L"Words left " << allWords.size() << L'\n';
			}
			else {
				std::wcout << L"Word " << amountOfRepeats + 1 << L" of " << wordsToUse << L'\n';
			}

			std::wcout << L"Leave empty to exit." << L'\n';
			std::wcout << word << L" is: ";

			wstring answer;
			if (wconsoleMenu::consoleWstringEditor(answer, findTimeToAnswer(word, translations))) {
				if (answer.empty()) {
					break;
				}

				wordToLowerCase(answer);
				transmitElement(word, unusedWords, usedWords);
				++amountOfRepeats;

				if (sucsessFeedback(answer, translations)) {
					correctAnswers++;
					overallCorrectAnswers[word]++;

					if (overallCorrectAnswers[word] >= ProgramDirectories::programProperties.correctAnswersToDelete.value) {
						MoveFileW(
							ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.unlearned).c_str(),
							ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.learned).c_str()
						);

						removeElement(word, usedWords);
						removeElement(word, unusedWords);
						overallCorrectAnswers.erase(word);
					}
				}
				else {
					--overallCorrectAnswers[word];
				}
			}
			else {
				_wsystem(L"cls");
				std::wcout << L"You run out of time!" << L'\n';
				printTranslations(translations);
				_wsystem(L"pause");

				transmitElement(word, unusedWords, usedWords);
				++amountOfRepeats;

				--overallCorrectAnswers[word];
			}

			_wsystem(L"cls");
		}

		if (amountOfRepeats > 0) {
			std::wcout << L"You was right in ";
			std::wcout << correctAnswers * 100 / amountOfRepeats;
			std::wcout << L"% of cases" << L'\n';

			saveMapToWfile(
				ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.successSave, currentLanguage),
				overallCorrectAnswers
			);
			_wsystem(L"pause");
		}

		saveVectorToWfile(
			ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.usedWords, currentLanguage),
			usedWords
		);
		_wsystem(L"cls");
	}
	else {
		while (allWords.size() > 0) {
			wstring word = randomWstringFromArray(allWords);
			if (word.empty()) {
				std::wcout << L"You have forgot all words!\n";
				_wsystem(L"pause");
				_wsystem(L"cls");
				break;
			}

			vector<wstring> translations;
			getVectorFromWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage), translations, true);
			if (translations.empty()) {
				std::wcout << L"Word \"" << word << L"\" doesn't exist already" << L'\n';
				_wsystem(L"pause");

				removeElement(word, allWords);
				continue;
			}

			std::wcout << L"Leave empty to exit." << L'\n';
			std::wcout << word << L" is: ";

			wstring answer;
			if (wconsoleMenu::consoleWstringEditor(answer, findTimeToAnswer(word, translations))) {
				if (answer.empty()) { break; }
				wordToLowerCase(answer);

				if (!sucsessFeedback(answer, translations))
				{
					MoveFileW(ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.learned).c_str(),
						ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.unlearned).c_str());
				}

				removeElement(word, allWords);
			}
			else {
				_wsystem(L"cls");
				std::wcout << L"You run out of time!" << L'\n';
				printTranslations(translations);
				_wsystem(L"pause");

				MoveFileW(ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.learned).c_str(),
					ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.unlearned).c_str());

				removeElement(word, allWords);
			}

			_wsystem(L"cls");
		}
	}
}

void partitionLearning(wstring&) {
	vector<wstring> allWords;

	getEntries(
		ProgramDirectories::getPathToDirectory(currentLanguage, currentStage),
		allWords
	);

	if (allWords.empty()) {
		std::wcout << L"Words not found!" << L'\n';
		_wsystem(L"pause");
		_wsystem(L"cls");
		return;
	}

	wcout << L"How many words in one partition would you like?\n";
	wcout << L"Enter amount from 1 to " << allWords.size() << L": ";

	int partitionSize;
	while (!(wcin >> partitionSize)) {
		continue;
	}

	wconsoleMenu displayWordsMenu = wconsoleMenu(allWords, L"Select start word");
	short startWordNumber = 0;
	displayWordsMenu.singleSelect(startWordNumber);


	vector<wstring> passedWords;
	int globalIndex = startWordNumber;
	do {
		vector<wstring> partitionWords;

		for (int i = 0; i < partitionSize; ++i) {
			wstring currentWord = allWords[globalIndex];

			partitionWords.push_back(currentWord);
			wordViewer(currentWord);
			_wsystem(L"cls");

			if (globalIndex + 1 < allWords.size()) {
				++globalIndex;
			}
			else {
				globalIndex = 0;
			}

			if (globalIndex == startWordNumber) {
				break;
			}
		}

		bool partitionPassed;
		do {
			partitionPassed = true;

			for (int i = 0; i < partitionWords.size(); ++i) {
				wstring word = partitionWords[i];

				vector<wstring> translations;
				getVectorFromWfile(
					ProgramDirectories::getPathToFile(word, currentLanguage, currentStage),
					translations
				);

				if (translations.empty()) {
					continue;
				}

				std::wcout << L"Leave empty to exit." << L'\n';
				std::wcout << word << L" is: ";

				wstring answer;
				wconsoleMenu::consoleWstringEditor(answer);

				if (answer.empty()) {
					partitionPassed = true;
					globalIndex = startWordNumber;
					break;
				}

				wordToLowerCase(answer);
				if (!sucsessFeedback(answer, translations)) {
					partitionPassed = false;
				}
				_wsystem(L"cls");
			}
		} while (!partitionPassed);

		for (int i = 0; i < partitionWords.size(); ++i) {
			passedWords.push_back(partitionWords[i]);
		}
	} while (globalIndex != startWordNumber && askQuestion(L"Next partition?"));

	if (askQuestion(L"Repeat all passed words?")) {
		for (int i = 0; i < passedWords.size(); i++) {
			wstring word = passedWords[i];

			vector<wstring> translations;
			getVectorFromWfile(
				ProgramDirectories::getPathToFile(word, currentLanguage, currentStage),
				translations
			);

			if (translations.empty()) {
				continue;
			}

			std::wcout << L"Leave empty to exit." << L'\n';
			std::wcout << word << L" is: ";

			wstring answer;
			wconsoleMenu::consoleWstringEditor(answer);

			if (answer.empty()) {
				break;
			}

			wordToLowerCase(answer);
			sucsessFeedback(answer, translations);
			_wsystem(L"cls");
		}
	}
}

void testingType(wstring& stage)
{
	currentStage = stage;

	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		vector<wstring> testingTypes = { L"Partition learning" , L"Open answer test" };
		vector<void (*)(wstring&)> functions = { partitionLearning , openAnswerTest };
		wstring selectText = L"Select a type of test";
		wstring exitText = L"Back";
		wconsoleMenu testingTypeMenu(testingTypes, functions, selectText, exitText);

		testingTypeMenu.cyclicSelect();
	}
	else { openAnswerTest(currentStage); }

	currentStage.clear();
}

void wordsStageToTest(wstring& language)
{
	vector<wstring> stageOfWords = { ProgramDirectories::stages.unlearned , ProgramDirectories::stages.learned };
	vector<void (*)(wstring&)> functions = functionMultiplier(testingType, stageOfWords.size());
	wstring selectText = L"Select a stage";
	wstring exitText = L"Back";
	wconsoleMenu testingTypeMenu(stageOfWords, functions, selectText, exitText);

	currentLanguage = language;
	testingTypeMenu.cyclicSelect();
	currentLanguage.clear();
}

void wordsLanguageToTest(wstring&)
{
	vector<wstring> languages = { ProgramDirectories::languages.native , ProgramDirectories::languages.target };
	vector<void (*)(wstring&)> functions = functionMultiplier(wordsStageToTest, languages.size());
	wstring selectText = L"Select a language";
	wstring exitText = L"Back";
	wconsoleMenu testingTypeMenu(languages, functions, selectText, exitText);

	testingTypeMenu.cyclicSelect();
}