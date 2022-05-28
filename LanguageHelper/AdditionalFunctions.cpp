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

void shuffleVector(vector<wstring>& vec)
{
	vector<wstring> auxiliaryVec = vec;
	vec.clear();

	while (!auxiliaryVec.empty())
	{
		int randomIndex = radnomNumber() % auxiliaryVec.size();
		vec.push_back(auxiliaryVec[randomIndex]);
		auxiliaryVec.erase(auxiliaryVec.begin() + randomIndex);
	}
}