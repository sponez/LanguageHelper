#include "AdditionalFunctions.h"
#include "wconsoleMenu.h"
#include "TestProcess.h"
#include "ManageWordsProcess.h"
#include "ViewProcess.h"
#include "Properties.h"
using namespace std;

int main(int argc, char* argv[])
{
	ProgramDirectories::getProgramDirectories();

	if (argc == 1)
	{
		vector<wstring> options = { L"View saved words" , L"Testing" , L"Manage words" , L"Properties" };
		vector<void (*)(wstring&)> functions = { wordsLanguageToView, wordsLanguageToTest , wordsLanguageToManage , propertiesMenu };
		wstring selectText = L"Select an action";
		wstring exitText = L"Close application";
		wconsoleMenu languageHelperMenu(options, functions, selectText, exitText);

		languageHelperMenu.cyclicSelect();
	}
	else if (argc == 2 && !strcmp(argv[1], "convert"))
	{
		vector<wstring> options = { ProgramDirectories::languages.native , ProgramDirectories::languages.target };
		wstring selectText = L"Select a sourse language:";
		wconsoleMenu languageHelperMenu(options, selectText);
		short optionNum = 0;

		languageHelperMenu.singleSelect(optionNum);
		if (optionNum == 0) { convertWordsFromAnotherLanguage(ProgramDirectories::languages.target); }
		else { convertWordsFromAnotherLanguage(ProgramDirectories::languages.native); }
	}
	else
	{
		wcout << L"An invalid command" << endl;
		return INT_MIN;
	}
}