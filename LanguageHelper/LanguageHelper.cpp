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
		languageHelperMenu.setMainMenuFlag();

		wconsoleMenu::setFontInfo(
			ProgramDirectories::programProperties.fontWeight.value,
			ProgramDirectories::programProperties.fontWidth.value,
			ProgramDirectories::programProperties.fontHeight.value
		);
		wconsoleMenu::setWindowSize(
			ProgramDirectories::programProperties.windowWidth.value,
			ProgramDirectories::programProperties.windowHeight.value
		);

		languageHelperMenu.cyclicSelect();
	}
	else
	{
		wcout << L"An invalid command" << endl;
		return INT_MIN;
	}
}