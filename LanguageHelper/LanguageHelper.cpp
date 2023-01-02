#include "AdditionalFunctions.h"
#include "wconsoleMenu.h"
#include "TestProcess.h"
#include "ManageWordsProcess.h"
#include "ViewProcess.h"
using namespace std;

int main(int argc, char* argv[])
{
	if (argc == 1)
	{
		converterFunction = false;
	}
	else if (argc == 2 && !strcmp(argv[1], "conv-on"))
	{
		converterFunction = true;
		wcout << L"Converter function: ";
		wcout << boolalpha;
		wcout << converterFunction;
		wcout << noboolalpha;
		wcout << endl;
		_wsystem(L"pause");
	}
	else
	{
		wcout << L"Invalid command" << endl;
		return INT_MIN;
	}

	ProgramDirectories::getProgramDirectories();

	vector<wstring> options = { L"View saved words" , L"Testing" , L"Manage words" };
	vector<void (*)(wstring&)> functions = { wordsLanguageToView, wordsLanguageToTest , wordsLanguageToManage };
	wconsoleMenu languageHelperMenu(options, functions, L"What do you want to do", L"Close application");
	languageHelperMenu.cyclicSelect();
}