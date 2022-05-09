#include "AdditionalFunctions.h"
#include "ViewProcess.h"
#include "UpdateProcess.h"
#include "DeleteProcess.h"
#include <array>
mt19937 radnomNumber((unsigned int)time(0));

void createDirrectory(wstring& path)
{
	struct _stat buf;
	if (_wstat(&path[0], &buf) != 0)
	{
		wstring command = L"md " + path;
		_wsystem(&command[0]);
	}
}

bool isFileExist(wstring& path)
{
	struct _stat buf;
	if (_wstat(&path[0], &buf) == 0)
		return true;
	else
		return false;
}

void wstringToLower(wstring& s)
{
	for (int i = 0; i < s.length(); i++)
		s[i] = tolower(s[i], locale(""));
}

void wstringStandartForm(wstring& s)
{
	wstringToLower(s);
	s[0] = toupper(s[0], locale(""));
}

void removeTextInBracket(wstring& string)
{
	int i = 0;

	while (i < string.length())
	{
		int leftBracket = -1;
		for (; i < string.length(); i++)
		{
			if (string[i] == L'(' && leftBracket == -1)
				leftBracket = i;

			if (string[i] == L')' && leftBracket > -1)
			{
				int lengthToRightBracket = i - leftBracket + 1;
				string.erase(leftBracket, lengthToRightBracket);
				i = leftBracket;
				break;
			}
		}
	}
}

double DamerauLevenshteinDistance(wstring& mainWord, wstring& secondaryWord)
{
	double mwl = mainWord.length();
	double swl = secondaryWord.length();
	if (mwl == 0.0 || swl == 0.0)
		return DBL_MAX;

	double deleteCost = 1.0;
	double insertCost = 1.0;
	double replaceCost = 1.0;
	double transposeCost = 0.5;
	double INF = mwl * swl;

	vector<vector<double>> distanceMatrix;
	distanceMatrix.resize(mwl + 1);
	for (int i = 0; i <= mwl; i++)
		distanceMatrix[i].resize(swl + 1);

	distanceMatrix[0][0] = INF;
	for (int i = 0; i < mwl; i++)
	{
		distanceMatrix[i + 1][1] = i * deleteCost;
		distanceMatrix[i + 1][0] = INF;
	}
	for (int j = 0; j < swl; j++)
	{
		distanceMatrix[1][j + 1] = j * insertCost;
		distanceMatrix[0][j + 1] = INF;
	}


	array<int, 65536> lastPosition;
	lastPosition.fill(0);

	for (int i = 1; i < mwl; i++)
	{
		double last = 0;
		for (int j = 1; j < swl; j++)
		{
			int _i = lastPosition[secondaryWord[j]];
			int _j = last;
			if (mainWord[i] == secondaryWord[j])
			{
				distanceMatrix[i + 1][j + 1] = distanceMatrix[i][j];
				last = j;
			}
			else
				distanceMatrix[i + 1][j + 1] = min(min((distanceMatrix[i][j] + replaceCost),
														(distanceMatrix[i + 1][j] + insertCost)),
														(distanceMatrix[i][j] + deleteCost));

			distanceMatrix[i + 1][j + 1] = min(distanceMatrix[i + 1][j + 1],
											(distanceMatrix[_i][_j] + (i - _i - 1) * deleteCost + transposeCost + (j - _j - 1) * insertCost));
		}

		lastPosition[mainWord[i]] = i;
	}
	return distanceMatrix[mwl][swl];
}

void getWords(wstring& path, vector<wstring>& emptyList)
{
	for (auto const& fileIterator : filesystem::directory_iterator{ path })
	{
		wstring word = fileIterator.path().filename();
		emptyList.push_back(word);
	}
}

void getWords(wstring& path, vector<wstring>& emptyList, wstring filterText)
{
	for (auto const& fileIterator : filesystem::directory_iterator{ path })
	{
		wstring word = fileIterator.path().filename();

		if (filterText.length() <= word.length() && filterText == word.substr(0, filterText.length()))
			emptyList.push_back(word);
	}
}

void displayFilteredWords(wstring& filterText)
{
	wstring startText = L"Filter by characters: ";

	vector<wstring> words;
	getWords(languageDirrectory, words, filterText);

	wcout << startText << filterText << endl;
	for (int i = 0; i < words.size(); i++)
		wcout << L"  " << words[i] << endl;

	COORD point;
	point.X = startText.length() + filterText.length();
	point.Y = 0;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

void displayAllSavedWords(wstring language)
{
	languageDirrectory = globalPath + L"\\" + language + L"Words";
	createDirrectory(languageDirrectory);

	wstring filterText = L"";
	displayFilteredWords(filterText);
	for (;; Sleep(10))
	{
		wchar_t wch;

		if (wch = _getwch())
		{
			if (wch == L'\r') break;

			if (wch == 8)
			{
				if (filterText.length() > 0)
				{
					_wsystem(L"cls");
					filterText.pop_back();
					displayFilteredWords(filterText);
				}
			}
			else
			{
				if ((char)wch == -32)
				{
					ignore = _getwch();
					continue;
				}

				_wsystem(L"cls");
				filterText += wch;
				displayFilteredWords(filterText);
			}
		}
	}

	vector<wstring> words;
	getWords(languageDirrectory, words, filterText);

	vector<void (*)(wstring)> displayFunctions;
	for (int i = 0; i < words.size(); i++)
	{
		if (updateOption == L"View existing words") displayFunctions.push_back(displayTranslationsFor);
		if (updateOption == L"Add translation") displayFunctions.push_back(addTranslationFunction);
		if (updateOption == L"Remove translation") displayFunctions.push_back(removeTranslationFunction);
		if (updateOption == L"Rewrite translations") displayFunctions.push_back(rewriteTranslationFunction);
		if (updateOption == L"Delete word") displayFunctions.push_back(deleteWordFunction);
	}

	filterText = L"Filter by characters: " + filterText;
	wconsoleMenu display(filterText, words, displayFunctions, L"*Back*");
	display.select();
}


wstring randomWordFrom(wstring& path)
{
	vector<wstring> words;
	getWords(path, words);

	size_t amountOfWords = words.size();
	if (amountOfWords == 0)
	{
		wcout << "Add some words at first" << endl;
		_wsystem(L"pause");
		return L"";
	}
	else
	{
		int randomIndex = radnomNumber() % amountOfWords;
		return words[randomIndex];
	}
}