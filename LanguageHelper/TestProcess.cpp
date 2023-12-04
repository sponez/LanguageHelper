#include "TestProcess.h"

wstring currentLanguage;
wstring currentStage;

double findTimeToAnswer(wstring& word, vector<wstring>& translations)
{
	if (ProgramDirectories::programProperties.millisecondsToAnswerForCharacter.value != INT_MAX)
	{
		const double timeToRead = 1200 + 60 * word.length();
		unsigned long long maxLen = 0;

		for (wstring translation : translations) {
			wstring translationWithoutBrackets = wstring(translation);
			cutTextInBracket(translationWithoutBrackets);

			maxLen = max(maxLen, translationWithoutBrackets.length());
		}

		return timeToRead + maxLen * ProgramDirectories::programProperties.millisecondsToAnswerForCharacter.value;
	}

	return INT_MAX;
}

void workOnMistakes(wstring&)
{
	vector<wstring> unpassedWords;
	map<wstring, int> overallCorrectAnswers;

	getVectorFromWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.unpassedWords, currentLanguage), unpassedWords);
	if (unpassedWords.empty())
	{
		wcout << L"No one mistake!" << endl;
		_wsystem(L"pause");
		return;
	}

	getMapFromWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.successSave, currentLanguage), overallCorrectAnswers);

	int currentWordIndex = 0;
	while (!unpassedWords.empty())
	{
		wstring word = unpassedWords[currentWordIndex];
		if (word.empty())
		{
			unpassedWords.erase(unpassedWords.begin() + currentWordIndex);
			overallCorrectAnswers.erase(word);
			continue;
		}

		vector<wstring> translations;
		getVectorFromWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage), translations);
		if (translations.empty())
		{
			wcout << L"Word \"" << word << L"\" doesn't exist already" << endl;
			_wsystem(L"pause");

			unpassedWords.erase(unpassedWords.begin() + currentWordIndex);
			overallCorrectAnswers.erase(word);
			continue;
		}

		wcout << L"Leave empty to exit." << endl;
		wcout << word << L" is: ";

		wstring answer;
		if (wconsoleMenu::consoleWstringEditor(answer, findTimeToAnswer(word, translations)))
		{
			if (answer.empty()) { break; }
			wordToLowerCase(answer);

			if (sucsessFeedback(answer, translations))
			{
				unpassedWords.erase(unpassedWords.begin() + currentWordIndex);
				overallCorrectAnswers.erase(word);

				if (unpassedWords.empty())
				{
					_wsystem(L"cls");
					wcout << L"All mistakes have fixed!" << endl;
					_wsystem(L"pause");
					break;
				}
			}
			else { currentWordIndex++; }
		}
		else
		{
			_wsystem(L"cls");
			wcout << L"You run out of time!" << endl;
			printTranslations(translations);
			_wsystem(L"pause");

			currentWordIndex++;
		}

		if (currentWordIndex >= unpassedWords.size()) { currentWordIndex = 0; }
		_wsystem(L"cls");
	}

	saveVectorToWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.unpassedWords, currentLanguage), unpassedWords);
	saveMapToWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.successSave, currentLanguage), overallCorrectAnswers);
	_wsystem(L"cls");
}

void openAnswerTest(wstring&)
{
	vector<wstring> allWords;

	getEntries(ProgramDirectories::getPathToDirectory(currentLanguage, currentStage), allWords);
	if (allWords.empty())
	{
		wcout << L"Words not found!" << endl;
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
		vector<wstring> unpassedWords;

		getVectorFromWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.unpassedWords, currentLanguage), unpassedWords);
		if (!unpassedWords.empty())
		{
			wcout << L"Fix mistakes and come back." << endl;
			_wsystem(L"pause");
			return;
		}

		getMapFromWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.successSave, currentLanguage), overallCorrectAnswers);
		getVectorFromWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.usedWords, currentLanguage), usedWords);
		vectorDifference(allWords, usedWords, unusedWords);
		for (int i = usedWords.size();; ++i)
		{
			if (i >= allWords.size())
			{
				unusedWords = vector<wstring>(usedWords);
				usedWords.clear();

				if (ProgramDirectories::programProperties.endlessTesting.value)
				{
					i = 0;
				}
				else
				{
					wcout << L"You have passed all words! ";
					if (ProgramDirectories::programProperties.randomTesting.value)
					{
						wcout << L"Start again to shuffle the words." << endl;
					}
					else
					{
						wcout << L"Start again to return to the first word." << endl;
					}

					_wsystem(L"pause");
					_wsystem(L"cls");
					break;
				}
			}

			wstring word;
			if (unusedWords.size() > 0)
			{
				if (ProgramDirectories::programProperties.randomTesting.value)
				{
					word = randomWstringFromArray(unusedWords);
				}
				else
				{
					word = unusedWords[0];
				}
			}
			else
			{
				wcout << L"Words are run out!" << endl;
				_wsystem(L"pause");
				_wsystem(L"cls");
				break;
			}

			vector<wstring> translations;
			getVectorFromWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage), translations);

			if (translations.empty())
			{
				wcout << L"Word doesn't exist already" << endl;
				_wsystem(L"pause");

				removeElement(word, usedWords);
				removeElement(word, unusedWords);
				overallCorrectAnswers.erase(word);
				continue;
			}

			wcout << L"Leave empty to exit." << endl;
			wcout << word << L" is: ";

			wstring answer;
			if (wconsoleMenu::consoleWstringEditor(answer, findTimeToAnswer(word, translations))) {
				if (answer.empty()) {
					break;
				}

				wordToLowerCase(answer);
				transmitElement(word, unusedWords, usedWords);
				amountOfRepeats++;
				if (sucsessFeedback(answer, translations)) {
					correctAnswers++;
					overallCorrectAnswers[word]++;

					if (overallCorrectAnswers[word] >= ProgramDirectories::programProperties.correctAnswersToDelete.value) {
						MoveFileW(ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.unlearned).c_str(),
							ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.learned).c_str());

						removeElement(word, usedWords);
						removeElement(word, unusedWords);
						overallCorrectAnswers.erase(word);
					}
				}
				else {
					overallCorrectAnswers[word] = 0;
				}
			}
			else
			{
				_wsystem(L"cls");
				wcout << L"You run out of time!" << endl;
				printTranslations(translations);
				_wsystem(L"pause");

				transmitElement(word, unusedWords, usedWords);
				amountOfRepeats++;
				overallCorrectAnswers[word] = 0;
			}

			_wsystem(L"cls");
		}

		if (amountOfRepeats > 0)
		{
			wcout << L"You was right in ";
			wcout << correctAnswers * 100 / amountOfRepeats;
			wcout << L"% of cases" << endl;

			if (correctAnswers < amountOfRepeats)
			{
				for (map<wstring, int>::iterator wordProgress = overallCorrectAnswers.begin(); wordProgress != overallCorrectAnswers.end(); wordProgress++)
				{
					if (wordProgress->second == 0)
					{
						unpassedWords.push_back(wordProgress->first);
					}
				}

				saveVectorToWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.unpassedWords, currentLanguage), unpassedWords);
			}

			saveMapToWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.successSave, currentLanguage), overallCorrectAnswers);
			_wsystem(L"pause");
		}

		saveVectorToWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.usedWords, currentLanguage), usedWords);
		_wsystem(L"cls");
	}
	else
	{
		
		while (allWords.size() > 0)
		{
			wstring word = randomWstringFromArray(allWords);
			if (word.empty())
			{
				wcout << L"You have forgot all words!" << endl;
				_wsystem(L"pause");
				_wsystem(L"cls");
				break;
			}

			vector<wstring> translations;
			getVectorFromWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage), translations, true);
			if (translations.empty())
			{
				wcout << L"Word \"" << word << L"\" doesn't exist already" << endl;
				_wsystem(L"pause");

				removeElement(word, allWords);
				continue;
			}

			wcout << L"Leave empty to exit." << endl;
			wcout << word << L" is: ";

			wstring answer;
			if (wconsoleMenu::consoleWstringEditor(answer, findTimeToAnswer(word, translations)))
			{
				if (answer.empty()) { break; }
				wordToLowerCase(answer);

				if (!sucsessFeedback(answer, translations))
				{
					MoveFileW(ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.learned).c_str(),
						ProgramDirectories::getPathToFile(word, currentLanguage, ProgramDirectories::stages.unlearned).c_str());
				}

				removeElement(word, allWords);
			}
			else
			{
				_wsystem(L"cls");
				wcout << L"You run out of time!" << endl;
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

void testingType(wstring& stage)
{
	currentStage = stage;

	if (currentStage == ProgramDirectories::stages.unlearned)
	{
		vector<wstring> testingTypes = { L"Open answer test" , L"Work on mistakes" };
		vector<void (*)(wstring&)> functions = { openAnswerTest , workOnMistakes };
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