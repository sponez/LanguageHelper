#include "AdditionalFunctions.h"

bool isPathExist(wstring path)
{
	struct _stat buf;
	if (_wstat(&path[0], &buf) == 0)
		return true;
	else
		return false;
}

void wordToLowerCase(wstring& word)
{
	for (int i = 0; i < word.length(); i++)
	{
		word[i] = tolower(word[i], locale(""));
	}
}

void capitalizedWord(wstring& word)
{
	wordToLowerCase(word);
	word[0] = toupper(word[0], locale(""));
}

wstring cutTextInBracket(wstring& string)
{
	int stringLenght = string.length();
	int leftBracketIndex = 0;
	int rightBracketIndex = stringLenght - 1;

	//Find left bracket index
	while (leftBracketIndex < stringLenght && string[leftBracketIndex] != L'(')
	{
		++leftBracketIndex;
	}

	//Find right bracket index
	while (rightBracketIndex >= 0 && string[rightBracketIndex] != L')')
	{
		--rightBracketIndex;
	}

	//Check if brackets aren't found
	if (leftBracketIndex == stringLenght || rightBracketIndex == -1)
	{
		return wstring();
	}
	else
	{
		//Find text in brackets
		wstring textInBracket = string.substr(leftBracketIndex + 1, rightBracketIndex - leftBracketIndex - 1);

		//Find left index to eraise from string
		int leftIndexToErase = leftBracketIndex;
		while (leftIndexToErase > 0 && string[leftIndexToErase - 1] == L' ')
		{
			--leftIndexToErase;
		}

		//Find right index to eraise from string
		int rightIndexToErase = rightBracketIndex + 1;
		while (rightIndexToErase < stringLenght && string[rightIndexToErase] == L' ')
		{
			++rightIndexToErase;
		}

		//Replace brackets with spaces before and after to one space
		wstring replacementText = (leftIndexToErase == 0 || rightIndexToErase == stringLenght) ? L"" : L" ";
		string.replace(string.begin() + leftIndexToErase, string.begin() + rightIndexToErase, replacementText);

		return textInBracket;
	}
}

void getEntries(wstring path, vector<wstring>& emptyList)
{
	if (!isPathExist(path)) {
		return;
	}

	for (auto const& fileIterator : filesystem::directory_iterator{ path }) {
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
		if (removeBracket) cutTextInBracket(word);
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

void removeWordFromWfile(wstring filePath, wstring word)
{
	vector<wstring> lines;

	getVectorFromWfile(filePath, lines);
	for (int i = 0; i < lines.size(); i++)
	{
		wstring rawLine = wstring(lines[i]);
		size_t separatorPosition = rawLine.find(L'>');

		if (separatorPosition != string::npos) {
			rawLine.substr(0, separatorPosition);
		}

		if (rawLine == word) {
			lines.erase(lines.begin() + i);
			saveVectorToWfile(filePath, lines);
			break;
		}
	}
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

bool askQuestion(wstring question)
{
	vector<wstring> options = { L"Yes" , L"No" };
	wconsoleMenu reserseAdd(options, question);
	short result = 0;
	reserseAdd.singleSelect(result);

	return (result == 0);
}

bool transmitElement(wstring element, vector<wstring>& sourseVector, vector<wstring>& targetvector)
{
	vector<wstring>::iterator elementIt = find(sourseVector.begin(), sourseVector.end(), element);

	if (elementIt != sourseVector.end())
	{
		sourseVector.erase(elementIt);
		targetvector.push_back(element);
		return true;
	}

	return false;
}

bool removeElement(wstring element, vector<wstring>& sourseVector)
{
	vector<wstring>::iterator elementIt = find(sourseVector.begin(), sourseVector.end(), element);

	if (elementIt != sourseVector.end())
	{
		sourseVector.erase(elementIt);
		return true;
	}

	return false;
}

wstring standartTranslationForm(wstring& translation, wstring& commentary)
{
	return commentary.empty() ? translation : translation + L" (" + commentary + L")";
}