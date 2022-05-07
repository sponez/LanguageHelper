#include "TestProcess.h"

void testFunction(wstring language)
{
	wstring languageDirrectory = globalPath + L"\\" + language + L"Words";
	createDirrectory(languageDirrectory);

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

		wstring word = randomWordFrom(languageDirrectory);
		if (word == L"") return;

		wifstream wordFile(languageDirrectory + L"\\" + word);
		wordFile.imbue(std::locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

		wcout << word << L" is: ";

		wstring answer;
		getline(wcin, answer);
		wstringToLower(answer);

		double maxMatchRate = 0.0;
		vector<wstring> translations;
		for (wstring anotherLanguageWord; getline(wordFile, anotherLanguageWord);)
		{
			removeTextInBracket(anotherLanguageWord);
			translations.push_back(anotherLanguageWord);
			maxMatchRate = max(maxMatchRate, matchWords(anotherLanguageWord, answer));
		}
		wordFile.close();

		if (maxMatchRate == 1.0)
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
		else if (maxMatchRate > 0.6)
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

void testingOption(wstring)
{
	vector<wstring> testingTypes = { firstLanguage, secondLanguage };
	vector<void (*)(wstring)> testingFunctions = { testFunction, testFunction };
	wconsoleMenu testingTypeMenu(L"Select the language of words you will translate", testingTypes, testingFunctions, L"I changed my mind. Back, please");
	testingTypeMenu.select();
}