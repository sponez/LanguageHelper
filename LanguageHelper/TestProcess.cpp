#include "TestProcess.h"
wstring currentLanguageDirrectory;

void multipleChoiceTest(wstring&)
{
	int amountOfRepeats;
	int amountOfAnswers;

	wcout << L"Enter amount of test repetitions (greater than zero): ";
	wcin >> amountOfRepeats;
	wcin.ignore(LLONG_MAX, '\n');

	if (amountOfRepeats <= 0)
	{
		wcout << L"Amount of test repetitions can't be less than one." << endl;
		_wsystem(L"pause");
		return;
	}

	wcout << L"Enter amount of answers in the test (greater than one and less than twenty one): ";
	wcin >> amountOfAnswers;
	wcin.ignore(LLONG_MAX, '\n');

	if (amountOfAnswers <= 1)
	{
		wcout << L"Amount of answers in the test can't be less than two." << endl;
		_wsystem(L"pause");
		return;
	}
	if (amountOfAnswers >= 21)
	{
		wcout << L"Amount of answers in the test can't be greater than twenty." << endl;
		_wsystem(L"pause");
		return;
	}

	int amountOfCorrectAnswers = 0;
	for (int i = 0; i < amountOfRepeats; i++)
	{
		wstring word = randomWordFrom(currentLanguageDirrectory);
		if (word == L"") return;

		vector<wstring> rightTranslations;
		getTranslations(currentLanguageDirrectory + L"\\" + word, rightTranslations);

		vector<wstring> answers;
		answers.push_back(rightTranslations[radnomNumber() % rightTranslations.size()]);

		for (int j = 0; j < amountOfAnswers - 1; j++)
		{
			vector<wstring> translations;
			getTranslations(currentLanguageDirrectory + L"\\" + randomWordFrom(currentLanguageDirrectory), translations);

			wstring answer = translations[radnomNumber() % translations.size()];
			answers.push_back(answer);
		}

		shuffleVector(answers);
		short answerNumber = 0;
		wconsoleMenu test(answers, word + L" is:");
		test.singleSelect(answerNumber);

		for (int j = 0; j < rightTranslations.size(); j++)
		{
			wcout << answers[answerNumber] << L'\t' << rightTranslations[j] << endl;

			if (rightTranslations[j] == answers[answerNumber])
			{
				amountOfCorrectAnswers++;
				wcout << L"You are right!" << endl;
				_wsystem(L"pause");
				break;
			}

			if (j == rightTranslations.size() - 1)
			{
				wcout << L"Wrong!" << endl;
				_wsystem(L"pause");
			}
		}
	}

	_wsystem(L"cls");
	wcout << L"You was right in " << amountOfCorrectAnswers * 100 / amountOfRepeats << L"% of cases" << endl;
	_wsystem(L"pause");
}

void openAnswerTest(wstring&)
{
	int amountOfRepeats;
	wcout << L"Enter amount of test repetitions (greater than zero): ";
	wcin >> amountOfRepeats;
	wcin.ignore(LLONG_MAX, '\n');

	if (amountOfRepeats <= 0)
	{
		wcout << L"Amount of test repetitions can't be less than one." << endl;
		_wsystem(L"pause");
		return;
	}

	int amountOfCorrectAnswers = 0;
	for (int i = 0; i < amountOfRepeats; i++)
	{
		_wsystem(L"cls");

		wstring word = randomWordFrom(currentLanguageDirrectory);
		if (word == L"") return;
		wcout << word << L" is: ";

		vector<wstring> translations;
		getTranslations(currentLanguageDirrectory + L"\\" + word, translations);

		wstring answer;
		getline(wcin, answer);
		wstringToLower(answer);

		double minDistance = DBL_MAX;
		if (minDistance == 0.0)
		{
			wcout << L"Absolutely correct!" << endl;

			if (translations.size() == 1)
			{
				wcout << L"Right is " << translations[0] << endl;
			}
			else
			{
				wcout << L"Right is one of:" << endl;
				for (int j = 0; j < translations.size(); j++)
					wcout << translations[j] << endl;
			}

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else if (minDistance <= 0.34)
		{
			wcout << L"Almost correct." << endl;

			if (translations.size() == 1)
			{
				wcout << L"Correct would be " << translations[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < translations.size(); j++)
					wcout << translations[j] << endl;
			}

			wcout << L"But it will be scored!" << endl;

			amountOfCorrectAnswers++;

			_wsystem(L"pause");
		}
		else
		{
			wcout << L"Nope." << endl;

			if (translations.size() == 1)
			{
				wcout << L"Correct would be " << translations[0] << endl;
			}
			else
			{
				wcout << L"Correct would be one of:" << endl;
				for (int j = 0; j < translations.size(); j++)
					wcout << translations[j] << endl;
			}

			wcout << L"It won't be scored!" << endl;

			_wsystem(L"pause");
		}
	}

	_wsystem(L"cls");
	wcout << L"You was right in " << amountOfCorrectAnswers * 100 / amountOfRepeats << L"% of cases" << endl;
	_wsystem(L"pause");
}

void testingType(wstring& language)
{
	currentLanguageDirrectory = globalPath + L"\\" + language + L" Words";
	createDirrectory(currentLanguageDirrectory);

	vector<wstring> testingTypes = { L"Open answer test" , L"Multiple choice test" };
	vector<void (*)(wstring&)> functions = { openAnswerTest , multipleChoiceTest };
	wconsoleMenu testingTypeMenu(testingTypes, functions, L"Select the type of test", L"I changed my mind. Back, please");
	testingTypeMenu.singleSelect();
}

void testingOption(wstring&)
{
	vector<wstring> testingTypes = { firstLanguage, secondLanguage };
	vector<void (*)(wstring&)> functions = functionMultiplier(testingType, testingTypes.size());
	wconsoleMenu testingTypeMenu(testingTypes, functions, L"Select the language of words you will translate", L"I changed my mind. Back, please");
	testingTypeMenu.singleSelect();
}