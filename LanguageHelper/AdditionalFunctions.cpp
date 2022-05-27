#include "AdditionalFunctions.h"
mt19937 radnomNumber((unsigned int)time(0));

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

void getWords(wstring& path, vector<wstring>& emptyList)
{
	for (auto const& fileIterator : filesystem::directory_iterator{ path })
	{
		wstring word = fileIterator.path().filename();
		emptyList.push_back(word);
	}
}

wstring randomWordFrom(wstring& path)
{
	vector<wstring> words;
	getWords(path, words);

	size_t amountOfWords = words.size();
	if (amountOfWords == 0)
	{
		wcout << L"Add some words at first" << endl;
		_wsystem(L"pause");
		return L"";
	}
	else
	{
		int randomIndex = radnomNumber() % amountOfWords;
		return words[randomIndex];
	}
}

vector<void (*)(wstring&)> functionMultiplier(void (*function)(wstring&), short amount)
{
	vector<void (*)(wstring&)> functions;

	for (short i = 0; i < amount; i++)
		functions.push_back(function);

	return functions;
}