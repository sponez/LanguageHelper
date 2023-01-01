#include "TestProcess.h"

wstring currentLanguage;
wstring currentStage;
const short AMOUNT_OF_CORRECT_ANSWERS_TO_DELETE = 3;

void workOnMistakes(wstring&)
{
	vector<wstring> unpassedWords;

	getVectorFromWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.unpassedWords, currentLanguage), unpassedWords);
	if (unpassedWords.empty())
	{
		wcout << L"No one mistake!" << endl;
		_wsystem(L"pause");
		return;
	}

	int currentWordIndex = 0;
	while (true)
	{
		wstring word = unpassedWords[currentWordIndex];

		vector<wstring> translations;
		wstring answer;

		wcout << L"Leave empty to exit." << endl;
		wcout << word << L" is: ";

		getline(wcin, answer);
		if (answer.empty()) { break; }
		wstringToLower(answer);

		getVectorFromWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage), translations, true);
		if (sucsessFeedback(answer, translations))
		{
			unpassedWords.erase(unpassedWords.begin() + currentWordIndex);
			if (unpassedWords.empty())
			{
				_wsystem(L"cls");
				wcout << L"All mistakes have fixed!" << endl;
				_wsystem(L"pause");
				break;
			}
		}
		else { currentWordIndex++; }

		if (currentWordIndex >= unpassedWords.size()) { currentWordIndex = 0; }

		_wsystem(L"cls");
	}

	saveVectorToWfile(ProgramDirectories::getPathToFile(ProgramDirectories::programFiles.unpassedWords, currentLanguage), unpassedWords);
	_wsystem(L"cls");
}

void openAnswerTest(wstring&)
{
	vector<wstring> allWords;

	getWords(ProgramDirectories::getPathToDirectory(currentLanguage, currentStage), allWords);
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
		while (true)
		{
			wstring word = randomWstring(unusedWords);
			if (word.empty())
			{
				_wsystem(L"cls");
				break;
			}

			usedWords.push_back(word);
			ignore = remove(unusedWords.begin(), unusedWords.end(), word);
			if (usedWords.size() > unusedWords.size())
			{
				unusedWords.push_back(usedWords.front());
				usedWords.erase(usedWords.begin());
			}

			vector<wstring> translations;
			wstring answer;

			wcout << L"Leave empty to exit." << endl;
			wcout << word << L" is: ";

			getline(wcin, answer);
			if (answer.empty()) { break; }
			wstringToLower(answer);

			amountOfRepeats++;
			getVectorFromWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage), translations, true);
			if (sucsessFeedback(answer, translations))
			{
				correctAnswers++;
				overallCorrectAnswers[word]++;
			}
			else
			{
				overallCorrectAnswers[word] = 0;
				unpassedWords.push_back(word);
			}

			if (overallCorrectAnswers[word] >= AMOUNT_OF_CORRECT_ANSWERS_TO_DELETE)
			{
				moveWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage),
					ProgramDirectories::getPathToDirectory(currentLanguage, ProgramDirectories::reverseStage(currentStage)));

				ignore = remove(usedWords.begin(), usedWords.end(), word);
				overallCorrectAnswers.erase(word);
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
		while (true)
		{
			wstring word = randomWstring(allWords);
			vector<wstring> translations;
			wstring answer;

			if (word.empty())
			{
				wcout << L"Words are run out" << endl;
				_wsystem(L"pause");
				_wsystem(L"cls");
				break;
			}

			wcout << L"Leave empty to exit." << endl;
			wcout << word << L" is: ";

			getline(wcin, answer);
			if (answer.empty())
			{
				_wsystem(L"cls");
				break;
			}
			wstringToLower(answer);

			getVectorFromWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage), translations, true);
			if (!sucsessFeedback(answer, translations))
			{
				moveWfile(ProgramDirectories::getPathToFile(word, currentLanguage, currentStage),
					ProgramDirectories::getPathToDirectory(currentLanguage, ProgramDirectories::reverseStage(currentStage)));

				ignore = remove(allWords.begin(), allWords.end(), word);
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
		vector<wstring> testingTypes = { L"Randomised test" , L"Work on mistakes" };
		vector<void (*)(wstring&)> functions = { openAnswerTest , workOnMistakes };
		wconsoleMenu testingTypeMenu(testingTypes, functions, L"Select the type of test", L"I changed my mind. Back, please");
		testingTypeMenu.cyclicSelect();
	}
	else
	{
		openAnswerTest(currentStage);
	}

	currentStage.clear();
}

void wordsStageToTest(wstring& language)
{
	currentLanguage = language;

	vector<wstring> testingTypes = { ProgramDirectories::stages.unlearned , ProgramDirectories::stages.learned };
	vector<void (*)(wstring&)> functions = functionMultiplier(testingType, testingTypes.size());
	wconsoleMenu testingTypeMenu(testingTypes, functions, L"Select the language of words you will translate", L"I changed my mind. Back, please");

	testingTypeMenu.singleSelect();

	currentLanguage.clear();
}

void wordsLanguageToTest(wstring&)
{
	vector<wstring> languages = { ProgramDirectories::languages.native , ProgramDirectories::languages.target };
	vector<void (*)(wstring&)> functions = functionMultiplier(wordsStageToTest, languages.size());
	wconsoleMenu testingTypeMenu(languages, functions, L"Select the language of words you will translate", L"I changed my mind. Back, please");

	testingTypeMenu.singleSelect();
}