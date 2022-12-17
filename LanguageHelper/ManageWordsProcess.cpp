#include "ManageWordsProcess.h"
#include <sstream>
wstring currentLanguageDirrectory;
wstring* currentWordPointer;
wstring* currentTranslationPointer;

void addWordFunction(wstring&)
{
	wstring word;
	wcout << L"Enter the word: ";
	getline(wcin, word);

	if (word.length() == 0)
	{
		wcout << L"Empty word" << endl;
		_wsystem(L"pause");
		return;
	}
	wstringToLower(word);

	wstring wordDirrectory = currentLanguageDirrectory + L"\\" + word;
	if (isFileExist(wordDirrectory))
	{
		wcout << L"This word already exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wofstream wordFile(currentLanguageDirrectory + L"\\" + word);
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
		wcout << "No one translations haven't entered." << endl;
		_wsystem(L"pause");

		wstring wordDirrectory = currentLanguageDirrectory + L"\\" + word;
		_wremove(wordDirrectory.c_str());
		return;
	}
}

void deleteWordFunction(wstring&)
{
	wstring currentWord = *currentWordPointer;
	wstring wordDirrectory = currentLanguageDirrectory + L"\\" + currentWord;

	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	_wremove(wordDirrectory.c_str());
	wcout << "Done!" << endl;
	_wsystem(L"pause");
	return;
}

void addTranslationFunction(wstring&)
{
	wstring currentWord = *currentWordPointer;
	wstring wordDirrectory = currentLanguageDirrectory + L"\\" + currentWord;
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

void translationFunction(wstring& optionName)
{
	wstring currentWord = *currentWordPointer;
	wstring wordDirrectory = currentLanguageDirrectory + L"\\" + currentWord;
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

				_wremove(wordDirrectory.c_str());
				return;
			}
		}
	}
	else
	{
		wcout << L"The translation already doesn't exist" << endl;
		return;
	}

	wofstream wordFileW(wordDirrectory);
	wordFileW.imbue(locale(wordFileW.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for (int i = 0; i < translations.size(); i++)
		wordFileW << translations[i] << endl;
	wordFileW.close();
}

void translatorEditor(wstring& translation)
{
	currentTranslationPointer = &translation;

	vector<wstring> editorOptions = { L"Edit" , L"Delete" };
	vector<void (*)(wstring&)> functions = functionMultiplier(translationFunction, editorOptions.size());
	wconsoleMenu languageOfWord(editorOptions, functions, L"What do you want to do?", L"I changed my mind. Back, please");
	languageOfWord.singleSelect();
}

void displayTranslations(wstring&)
{
	wstring currentWord = *currentWordPointer;
	wstring wordDirrectory = currentLanguageDirrectory + L"\\" + currentWord;
	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	short optionNum = 0;
	wstring filterText = L"";
	bool skipFilter = false;
	do
	{
		vector<wstring> translations;
		vector<void (*)(wstring&)> functions;
		wconsoleMenu displayTranslations;

		wifstream wordFileR(wordDirrectory);
		wordFileR.imbue(locale(wordFileR.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
		for (wstring translation; getline(wordFileR, translation);)
			translations.push_back(translation);
		wordFileR.close();

		if (translations.size() == 0)
		{
			wcout << "Translations not found! The word will be deleted." << endl;
			_wsystem(L"pause");

			_wremove(wordDirrectory.c_str());
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
	wstring currentWord = *currentWordPointer;
	wstring wordDirrectory = currentLanguageDirrectory + L"\\" + currentWord;
	if (!isFileExist(wordDirrectory))
	{
		wcout << L"This word already doesn't exist." << endl;
		_wsystem(L"pause");
		return;
	}

	wifstream wordFileR(wordDirrectory);
	wordFileR.imbue(locale(wordFileR.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	vector<wstring> translations;
	for (wstring translation; getline(wordFileR, translation);)
		translations.push_back(translation);
	wordFileR.close();

	if (translations.size() == 0)
	{
		wcout << "Translations not found! The word is broken and will be deleted." << endl;
		_wsystem(L"pause");
		
		_wremove(wordDirrectory.c_str());
		return;
	}

	wconsoleMenu::consoleWstringEditor(*currentWordPointer);
	currentWord = *currentWordPointer;
	_wremove(wordDirrectory.c_str());
	wordDirrectory = currentLanguageDirrectory + L"\\" + currentWord;

	wofstream wordFileW(wordDirrectory);
	wordFileW.imbue(locale(wordFileW.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for (int i = 0; i < translations.size(); i++)
		wordFileW << translations[i] << endl;
	wordFileW.close();
}

void wordEditor(wstring& word)
{
	currentWordPointer = &word;

	vector<wstring> editorOptions = { L"Rename" , L"View translations" , L"Add translations" , L"Delete"};
	vector<void (*)(wstring&)> functions = { ranameWordFunction , displayTranslations , addTranslationFunction , deleteWordFunction };
	wconsoleMenu editor(editorOptions, functions, L"What do you want to do?", L"I changed my mind. Back, please");

	short optionNum = 0;
	do
	{
		editor.singleSelect(optionNum);
	} while (optionNum != -1 && optionNum != 3);
}

void displayAllSavedWordsToEdit(wstring& language)
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

		getWords(currentLanguageDirrectory, words);
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
			displayWords.singleSelect(optionNum);
		}

	} while (optionNum != -1);
}

void addNewOrView(wstring& language)
{
	currentLanguageDirrectory = globalPath + L"\\Unlearned\\" + language + L" Words";
	createDirrectory(currentLanguageDirrectory);

	vector<wstring> options = { L"Edit saved words" , L"Add new word" };
	vector<void (*)(wstring&)> functions = { displayAllSavedWordsToEdit , addWordFunction };
	wconsoleMenu languageOfWord(options, functions, L"What do you want to do?", L"I changed my mind. Back, please");
	languageOfWord.cyclicSelect();

	currentLanguageDirrectory.clear();
}

void manageWordsOption(wstring&)
{
	vector<wstring> languages = { firstLanguage, secondLanguage };
	vector<void (*)(wstring&)> functions = functionMultiplier(addNewOrView, languages.size());
	wconsoleMenu languageOfWord(languages, functions, L"Select the language of the saved words you want to manage", L"I changed my mind. Back, please");
	languageOfWord.singleSelect();
}