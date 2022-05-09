#include "UpdateProcess.h"

void addTranslationFunction(wstring word)
{
	wstring wordDirrectory = languageDirrectory + L"\\" + word;
	createDirrectory(wordDirrectory);

	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wofstream wordFile(wordDirrectory, ios::app);
	wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	wstring translation = L" ";
	while (translation.length() > 0)
	{
		wcout << L"Enter a translation (if you want to exit just leave it empty): ";
		getline(wcin, translation);
		wstringToLower(translation);

		if (translation.length() > 0)
			wordFile << translation << endl;
	}
	wordFile.close();
}

void removeTranslationFunction(wstring word)
{
	wstring wordDirrectory = languageDirrectory + L"\\" + word;
	createDirrectory(wordDirrectory);

	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	vector<wstring> translations;
	wifstream wordFileR(wordDirrectory);
	wordFileR.imbue(locale(wordFileR.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for (wstring translation; getline(wordFileR, translation);)
		translations.push_back(translation);
	wordFileR.close();


	wstring translationToRemove = L" ";
	while (translationToRemove.length() > 0)
	{
		wcout << L"Enter the translation you want to remove (if you want to exit just leave it empty): ";
		getline(wcin, translationToRemove);
		wstringToLower(translationToRemove);

		vector<wstring>::iterator positionOfTranslationToRemove = find(translations.begin(), translations.end(), translationToRemove);
		if (positionOfTranslationToRemove != translations.end())
		{
			translations.erase(positionOfTranslationToRemove);

			if (translations.size() == 0)
			{
				wcout << L"You have removed all existed translations. The word will be deleted." << endl;
				_wsystem(L"pause");

				wstring command = L"del " + wordDirrectory;
				_wsystem(&command[0]);
				return;
			}
		}
		else
		{
			wcout << L"The translation doesn't exist. Try again." << endl;
		}
	}

	wofstream wordFileW(wordDirrectory);
	wordFileW.imbue(locale(wordFileW.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for (int i = 0; i < translations.size(); i++)
		wordFileW << translations[i] << endl;
	wordFileW.close();
}

void rewriteTranslationFunction(wstring word)
{
	wstring wordDirrectory = languageDirrectory + L"\\" + word;
	createDirrectory(wordDirrectory);

	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wofstream wordFile(wordDirrectory);
	wordFile.imbue(locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	wstring translation = L" ";
	vector<wstring> translations;
	while (translation.length() > 0)
	{
		wcout << L"Enter a translation (if you want to exit just leave it empty): ";
		getline(wcin, translation);
		wstringToLower(translation);

		if (translation.length() > 0)
		{
			translations.push_back(translation);
			wordFile << translation << endl;
		}
	}
	wordFile.close();

	if (translations.size() == 0)
	{
		wcout << "No one translations haven't entered. The word will be removed." << endl;
		_wsystem(L"pause");

		wstring command = L"del " + wordDirrectory;
		_wsystem(&command[0]);
		return;
	}
}

void updateWordOptionLanguage(wstring optionName)
{
	updateOption = optionName;

	vector<wstring> languages = { firstLanguage, secondLanguage };
	vector<void (*)(wstring)> displayWordFunctions = { displayAllSavedWords, displayAllSavedWords };
	wconsoleMenu languageOfWord(L"Select the language of the word you want to update", languages, displayWordFunctions, L"I changed my mind. Back, please");
	languageOfWord.select();
}

void updateWordOption(wstring)
{
	vector<wstring> options = { L"Add translation", L"Remove translation", L"Rewrite translations" };
	vector<void (*)(wstring)> updateWordFunctions = { updateWordOptionLanguage, updateWordOptionLanguage, updateWordOptionLanguage };
	wconsoleMenu update(L"Select the option", options, updateWordFunctions, L"I changed my mind. Back, please");
	update.select();
}