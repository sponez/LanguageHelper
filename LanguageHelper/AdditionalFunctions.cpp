#include "AdditionalFunctions.h"

bool isPathExist(wstring path)
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

void removeTextInBracket(wstring& string)
{
	for (int i = 0; i < string.length(); i++)
	{
		if (string[i] == L'(')
		{
			string.erase(string.begin() + i, string.end());
			break;
		}
	}

	while (string.back() == L' ') { string.pop_back(); }
}

void getWords(wstring path, vector<wstring>& emptyList)
{
	if (!isPathExist(path)) return;

	for (auto const& fileIterator : filesystem::directory_iterator{ path })
	{
		wstring word = fileIterator.path().filename();
		emptyList.push_back(word);
	}
}

void vectorDifference(vector<wstring>& first, vector<wstring>& second, vector<wstring>& result)
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

wstring randomWstring(vector<wstring>& wstringArray)
{
	if (wstringArray.empty())
	{
		return wstring();
	}
	else
	{
		int randomIndex = radnomNumber() % wstringArray.size();
		return wstringArray[randomIndex];
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

void getVectorFromWfile(wstring filePath, vector<wstring>& emptyVector, bool removeBracket)
{
	if (!isPathExist(filePath)) return;

	wifstream sourseFile(filePath);
	sourseFile.imbue(locale(sourseFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	for (wstring word; getline(sourseFile, word);)
	{
		if (removeBracket) removeTextInBracket(word);
		emptyVector.push_back(word);
	}

	sourseFile.close();
}

void saveVectorToWfile(wstring filePath, vector<wstring>& sourseVector)
{
	wofstream targetFile(filePath);
	targetFile.imbue(locale(targetFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	for (wstring word: sourseVector)
	{
		targetFile << word << endl;
	}

	targetFile.close();
}

void getMapFromWfile(wstring filePath, map<wstring, int>& emptyMap)
{
	if (!isPathExist(filePath)) return;

	wifstream sourseFile(filePath);
	sourseFile.imbue(std::locale(sourseFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	for (wstring wstr; getline(sourseFile, wstr, L'>');)
	{
		wstring counter;
		getline(sourseFile, counter);
		emptyMap.insert(pair<wstring, int>(wstr, stoi(counter)));
	}

	sourseFile.close();
}

void saveMapToWfile(wstring filePath, map<wstring, int>& sourseMap)
{
	wofstream targetFile(filePath);
	targetFile.imbue(std::locale(targetFile.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));

	for (map<wstring, int>::iterator it = sourseMap.begin(); it != sourseMap.end(); it++)
		targetFile << it->first << L'>' << it->second << endl;

	targetFile.close();
}

void moveWfile(wstring soursePath, wstring targetPath)
{
	if (!isPathExist(soursePath)) return;

	wstring command = L"move \"" + soursePath + L"\" \"" + targetPath + L"\"";
	_wsystem(command.c_str());
}

void printVector(vector<wstring> vector, wstring separator, bool newLineAfter)
{
	if (vector.empty()) return;

	for (int i = 0; i < vector.size() - 1; i++) { wcout << vector[i] << separator; }
	wcout << vector.back();
	if (newLineAfter) wcout << endl;
}

void printTranslations(vector<wstring> translations)
{
	if (translations.size() == 1) { wcout << L"Correct is " << translations[0] << endl; }
	else
	{
		wcout << L"One of the following is correct:" << endl;
		printVector(translations);
	}
}

bool sucsessFeedback(wstring answer, vector<wstring> translations)
{
	double minDistance = DBL_MAX;

	for (wstring translation : translations)
	{
		double currentDistance = wconsoleMenu::DamerauLevenshteinDistance(translation, answer) / translation.length();
		minDistance = min(minDistance, currentDistance);
	}

	if (minDistance == 0.0)
	{
		wcout << L"Absolutely correct!" << endl;
		printTranslations(translations);
		_wsystem(L"pause");

		return true;
	}
	else if (minDistance <= 0.34)
	{
		wcout << L"Almost correct." << endl;
		printTranslations(translations);
		wcout << L"But it will be scored!" << endl;
		_wsystem(L"pause");

		return true;
	}
	else
	{
		wcout << L"Nope." << endl;
		printTranslations(translations);
		wcout << L"It won't be scored!" << endl;
		_wsystem(L"pause");

		return false;
	}
}

void addAllPairsCorrespondencesToSetFrom(wstring path, set<pair<wstring, wstring>>& correspondences, bool reverseOrder)
{
	if (!isPathExist(path)) return;

	for (auto const& fileIterator : filesystem::directory_iterator{ path })
	{
		wstring word = fileIterator.path().filename();
		wstring wordPath = path + L"\\" + word;
		vector<wstring> translations;

		getVectorFromWfile(wordPath, translations, true);
		for (wstring translation : translations)
		{
			if (reverseOrder)
			{
				correspondences.insert(pair<wstring, wstring>(translation, word));
			}
			else
			{
				correspondences.insert(pair<wstring, wstring>(word, translation));
			}
		}
	}
}

bool contains(vector<wstring>& vector, wstring& element)
{
	return (find(vector.begin(), vector.end(), element) != vector.end());
}