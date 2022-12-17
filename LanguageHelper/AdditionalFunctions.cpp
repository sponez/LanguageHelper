#include "AdditionalFunctions.h"

void createDirrectory(wstring& path)
{
	struct _stat buf;
	if (_wstat(&path[0], &buf) != 0)
	{
		wstring command = L"md \"" + path + L'\"';
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

		while (string[string.length() - 1] == L' ')
			string.pop_back();
	}
}

void getWords(wstring path, vector<wstring>& emptyList)
{
	emptyList.clear();
	for (auto const& fileIterator : filesystem::directory_iterator{ path })
	{
		wstring word = fileIterator.path().filename();
		emptyList.push_back(word);
	}
}

void getTranslations(wstring path, vector<wstring>& emptyList)
{
	emptyList.clear();
	wifstream wordFile(path);
	wordFile.imbue(std::locale(wordFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	for (wstring anotherLanguageWord; getline(wordFile, anotherLanguageWord);)
	{
		removeTextInBracket(anotherLanguageWord);
		emptyList.push_back(anotherLanguageWord);
	}
	wordFile.close();
}

void vectorDifference(vector<wstring> &first, vector<wstring> &second, vector<wstring> &result)
{
	for (int i = 0; i < first.size(); i++)
	{
		bool match = false;

		for (int j = 0; j < second.size(); j++)
		{
			if (first[i] == second[j])
			{
				match = true;
				break;
			}
		}

		if (!match) result.push_back(first[i]);
	}
}

wstring randomWordFrom(wstring& path, bool saveWord)
{
	static vector<wstring> usedWords;
	vector<wstring> words;
	size_t amountOfWords;
	getWords(path, words);

	if (words.size() == 0)
	{
		wcout << L"Add some words at first" << endl;
		_wsystem(L"pause");
		return L"";
	}
	else if (words.size() == 1)
	{
		return words[0];
	}
	else
	{
		vector<wstring> unusedWords;
		vectorDifference(words, usedWords, unusedWords);

		amountOfWords = unusedWords.size();
		int randomIndex = radnomNumber() % amountOfWords;
		wstring selectedWord = unusedWords[randomIndex];

		if (saveWord) usedWords.push_back(selectedWord);
		if (2 * usedWords.size() >= unusedWords.size()) usedWords.erase(usedWords.begin());

		return selectedWord;
	}
}

vector<void (*)(wstring&)> functionMultiplier(void (*function)(wstring&), short amount)
{
	vector<void (*)(wstring&)> functions;

	for (short i = 0; i < amount; i++)
		functions.push_back(function);

	return functions;
}

void shuffleVector(vector<wstring>& vectorForShuffle)
{
	vector<wstring> auxiliaryVector = vectorForShuffle;
	vectorForShuffle.clear();

	while (!auxiliaryVector.empty())
	{
		int randomIndex = radnomNumber() % auxiliaryVector.size();
		vectorForShuffle.push_back(auxiliaryVector[randomIndex]);
		auxiliaryVector.erase(auxiliaryVector.begin() + randomIndex);
	}
}